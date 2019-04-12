/*****************************************************************************
 * Copyright [2017] [taurus.ai]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

/**
 * MDEngineCTP: CTP's market data engine adapter.
 * @Author cjiang (changhao.jiang@taurus.ai)
 * @since   April, 2017
 */

#include "MDEngineCTP.h"
#include "TypeConvert.hpp"
#include "Timer.h"
#include "longfist/ctp.h"
#include "longfist/LFUtils.h"

USING_WC_NAMESPACE

MDEngineCTP::MDEngineCTP(): IMDEngine(SOURCE_CTP), api(nullptr), connected(false), logged_in(false), reqId(0)
{
    logger = yijinjing::KfLog::getLogger("MdEngine.CTP");
}

void MDEngineCTP::load(const json& j_config)
{
    broker_id = j_config[WC_CONFIG_KEY_BROKER_ID].get<string>();
    user_id = j_config[WC_CONFIG_KEY_USER_ID].get<string>();
    password = j_config[WC_CONFIG_KEY_PASSWORD].get<string>();
    front_uri = j_config[WC_CONFIG_KEY_FRONT_URI].get<string>();
}

void MDEngineCTP::connect(long timeout_nsec)
{
    if (api == nullptr)
    {
        api = CThostFtdcMdApi::CreateFtdcMdApi();
		KF_LOG_INFO(logger, "[request] connect begin...");
        if (!api)
        {
        	KF_LOG_ERROR(logger, "[request] connect failed!");
            throw std::runtime_error("CTP_MD failed to create api");
        }
        api->RegisterSpi(this);
    }
    if (!connected)
    {
        api->RegisterFront((char*)front_uri.c_str());
        api->Init();
        long start_time = yijinjing::getNanoTime();
		KF_LOG_INFO(logger, "[request] connect timeout " << timeout_nsec);
        while (!connected && yijinjing::getNanoTime() - start_time < 2 * timeout_nsec)
        {}
    }
	
	KF_LOG_INFO(logger, "[request] connect end");
	connected = true;
}

void MDEngineCTP::login(long timeout_nsec)
{
	KF_LOG_INFO(logger, "[login]");

    if (!logged_in)
    {
    	KF_LOG_ERROR(logger, "[request] login begin...");
        CThostFtdcReqUserLoginField req = {};
        strcpy(req.BrokerID, broker_id.c_str());
        strcpy(req.UserID, user_id.c_str());
        strcpy(req.Password, password.c_str());
        if (api->ReqUserLogin(&req, reqId++))
        {
            KF_LOG_ERROR(logger, "[request] login failed!" << " (Bid)" << req.BrokerID
                                                           << " (Uid)" << req.UserID);
        }
        long start_time = yijinjing::getNanoTime();
        while (!logged_in && yijinjing::getNanoTime() - start_time < timeout_nsec)
        {}
    }
}

void MDEngineCTP::logout()
{
	KF_LOG_INFO(logger, "[logout]");

    if (logged_in)
    {
        CThostFtdcUserLogoutField req = {};
        strcpy(req.BrokerID, broker_id.c_str());
        strcpy(req.UserID, user_id.c_str());
        if (api->ReqUserLogout(&req, reqId++))
        {
            KF_LOG_ERROR(logger, "[request] logout failed!" << " (Bid)" << req.BrokerID
                                                            << " (Uid)" << req.UserID);
        }
    }
    connected = false;
    logged_in = false;
}

void MDEngineCTP::release_api()
{
	KF_LOG_INFO(logger, "[release_api]");

    if (api != nullptr)
    {
        api->Release();
        api = nullptr;
    }
}

void MDEngineCTP::subscribeMarketData(const vector<string>& instruments, const vector<string>& markets)
{
	KF_LOG_INFO(logger, "[subscribeMarketData]");

    int nCount = instruments.size();
    char* insts[nCount];
    for (int i = 0; i < nCount; i++)
        insts[i] = (char*)instruments[i].c_str();
    api->SubscribeMarketData(insts, nCount);
}

/*
 * SPI functions
 */
void MDEngineCTP::OnFrontConnected()
{
    KF_LOG_INFO(logger, "[OnFrontConnected]");
    connected = true;
}

void MDEngineCTP::OnFrontDisconnected(int nReason)
{
    KF_LOG_INFO(logger, "[OnFrontDisconnected] reason=" << nReason);
    connected = false;
    logged_in = false;
}

#define GBK2UTF8(msg) kungfu::yijinjing::gbk2utf8(string(msg))

void MDEngineCTP::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
    {
        KF_LOG_ERROR(logger, "[OnRspUserLogin]" << " (errID)" << pRspInfo->ErrorID
                                                << " (errMsg)" << GBK2UTF8(pRspInfo->ErrorMsg));
    }
    else
    {
        KF_LOG_INFO(logger, "[OnRspUserLogin]" << " (Bid)" << pRspUserLogin->BrokerID
                                               << " (Uid)" << pRspUserLogin->UserID
                                               << " (SName)" << pRspUserLogin->SystemName);
        logged_in = true;
    }
}

void MDEngineCTP::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
    {
        KF_LOG_ERROR(logger, "[OnRspUserLogout]" << " (errID)" << pRspInfo->ErrorID
                                                 << " (errMsg)" << GBK2UTF8(pRspInfo->ErrorMsg));
    }
    else
    {
        KF_LOG_INFO(logger, "[OnRspUserLogout]" << " (Bid)" << pUserLogout->BrokerID
                                                << " (Uid)" << pUserLogout->UserID);
        logged_in = false;
    }
}

void MDEngineCTP::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	KF_LOG_INFO(logger, "[OnRspSubMarketData] Return");
    if (pRspInfo != nullptr && pRspInfo->ErrorID != 0)
    {
        KF_LOG_ERROR(logger, "[OnRspSubMarketData]" << " (errID)" << pRspInfo->ErrorID
                                                    << " (errMsg)" << GBK2UTF8(pRspInfo->ErrorMsg)
                                                    << " (Tid)" << ((pSpecificInstrument != nullptr) ?
                                                                    pSpecificInstrument->InstrumentID : "null"));
    }
}

void MDEngineCTP::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	KF_LOG_INFO(logger, "[OnRtnDepthMarketData] Return");
	if (pDepthMarketData == NULL) {
		KF_LOG_INFO(logger, "[OnRtnDepthMarketData] pDepthMarketData == NULL");
	}
	
	KF_LOG_INFO(logger, "ori.TradingDay " << pDepthMarketData->TradingDay << 
					"ori.TradingDay " << pDepthMarketData->InstrumentID <<
					"ori.ExchangeID " << pDepthMarketData->ExchangeID);
	
    auto data = parseFrom(*pDepthMarketData);
    on_market_data(&data);
    // if need to write raw data...
    // raw_writer->write_frame(pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField),
    //                         source_id, MSG_TYPE_LF_MD_CTP, 1/*islast*/, -1/*invalidRid*/);
}

BOOST_PYTHON_MODULE(libctpmd)
{
    using namespace boost::python;
    class_<MDEngineCTP, boost::shared_ptr<MDEngineCTP> >("Engine")
    .def(init<>())
    .def("init", &MDEngineCTP::initialize)
    .def("start", &MDEngineCTP::start)
    .def("stop", &MDEngineCTP::stop)
    .def("logout", &MDEngineCTP::logout)
    .def("wait_for_stop", &MDEngineCTP::wait_for_stop);
}
