#include <iostream>
#include <cstdlib>
#include <string>
#include <signal.h>

#include <document.h>
#include <writer.h>
#include <stringbuffer.h>
#include <chrono>



#include <writer.h>
#include <stringbuffer.h>
#include <document.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <assert.h>



#include "../../utils/crypto/openssl_util.h"


using rapidjson::StringRef;
using rapidjson::Writer;
using rapidjson::StringBuffer;
using rapidjson::Document;
using rapidjson::SizeType;
using rapidjson::Value;
using std::string;
using std::to_string;
using std::stod;
using std::stoi;
using namespace rapidjson;
using namespace std;

#include <libwebsockets.h>

using utils::crypto::hmac_sha384;
//api.bitfinex.com/ws/2
//bitmax.io/

static std::string host  = "api.bitfinex.com";
static std::string path = "/ws/2";
static int port = 443;


static void on_lws_data(struct lws* conn, const char* data, size_t len) {
    std::cout << "on_lws_data: " << data << std::endl;
}

static void on_lws_connection_error(struct lws* conn) {
    std::cout << "on_lws_connection_error:"<< std::endl;
}


static int64_t getTimestamp()
{
    long long timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return timestamp;
}


/*
 * {
        event: "auth",
        apiKey: api_key,
        authSig: signature,
        authPayload: payload,
        authNonce: +authNonce,
        calc: 1
    }

 * */
std::string createAuthJsonString()
{
    std::string authNonce = std::to_string(getTimestamp());

    std::string secret_key = "v2jejpDKpcWmQYVMSXMYkFoGSBb3mguVgOr1C2YFo9q";

    std::string payload = "AUTH" + authNonce;

    std::string signature =  hmac_sha384( secret_key.c_str(), payload.c_str());

    std::cout << "payload:" << payload << std::cout;
    std::cout << "signature:" << signature << std::cout;


    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("event");
    writer.String("auth");

    writer.Key("apiKey");
    writer.String("yZ7P9B8akLetIOg1M5ixbp3sUpVhs2D8yJAA7uhciU7");

    writer.Key("authSig");
    writer.String(signature.c_str());

    writer.Key("authPayload");
    writer.String(payload.c_str());

    writer.Key("authNonce");
    writer.String(authNonce.c_str());

    writer.Key("dms");
    writer.Int(4);

    writer.EndObject();
    return s.GetString();
}

static int lws_write_subscribe(struct lws* conn)
{
    std::cout << "lws_write_subscribe"<< std::endl;




    unsigned char msg[512];
    memset(&msg[LWS_PRE], 0, 512-LWS_PRE);
    std::string jsonString = createAuthJsonString();

    std::cout << "lws_write_subscribe: " << jsonString.c_str() << std::endl;
    int length = jsonString.length();

    strncpy((char *)msg+LWS_PRE, jsonString.c_str(), length);
    int ret = lws_write(conn, &msg[LWS_PRE], length, LWS_WRITE_TEXT);

    return ret;
}


static int lws_event_cb( struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{

    switch( reason )
    {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
        {
            std::cout << "3.1415926 LWS_CALLBACK_CLIENT_ESTABLISHED callback client established, reason = " << reason << std::endl;
            lws_callback_on_writable( wsi );
            break;
        }
        case LWS_CALLBACK_PROTOCOL_INIT:{
            std::cout << "3.1415926 LWS_CALLBACK_PROTOCOL_INIT init, reason = " << reason << std::endl;
            break;
        }
        case LWS_CALLBACK_CLIENT_RECEIVE:
        {
            std::cout << "3.1415926 LWS_CALLBACK_CLIENT_RECEIVE on data, reason = " << reason << std::endl;

            {
                on_lws_data(wsi, (const char*)in, len);
            }
            break;
        }
        case LWS_CALLBACK_CLIENT_CLOSED:
        {
            std::cout << "3.1415926 LWS_CALLBACK_CLIENT_CLOSED, reason = " << reason << std::endl;

            {
                std::cout << "3.1415926 LWS_CALLBACK_CLIENT_CLOSED 2,  (call on_lws_connection_error)  reason = " << reason << std::endl;
                on_lws_connection_error(wsi);
            }

            break;
        }
        case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
        {
            std::cout << "3.1415926 LWS_CALLBACK_CLIENT_RECEIVE_PONG, reason = " << reason << std::endl;
            break;
        }
        case LWS_CALLBACK_CLIENT_HTTP_WRITEABLE:
        {
            std::cout << "3.1415926 LWS_CALLBACK_CLIENT_HTTP_WRITEABLE writeable, reason = " << reason << std::endl;
            /**< when doing an HTTP type client connection, you can call
             * lws_client_http_body_pending(wsi, 1) from
             * LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER to get these callbacks
             * sending the HTTP headers.
             *
             * From this callback, when you have sent everything, you should let
             * lws know by calling lws_client_http_body_pending(wsi, 0)
             */
            break;
        }
        case LWS_CALLBACK_CLIENT_WRITEABLE:
        {
            std::cout << "3.1415926 LWS_CALLBACK_CLIENT_WRITEABLE writeable, reason = " << reason << std::endl;

            {
                lws_write_subscribe(wsi);
            }
            break;
        }
        case LWS_CALLBACK_TIMER:
        {
            std::cout << "3.1415926 LWS_CALLBACK_TIMER, reason = " << reason << std::endl;
            break;
        }
        case LWS_CALLBACK_CLOSED:
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        {
            std::cout << "3.1415926 LWS_CALLBACK_CLOSED,LWS_CALLBACK_CLIENT_CONNECTION_ERROR reason = " << reason << std::endl;

            {
                on_lws_connection_error(wsi);
            }
            break;
        }
        default:
            break;
    }

    return 0;
}



static struct lws_protocols protocols[] =
        {
                {
                        "md-protocol",
                        lws_event_cb,
                              0,
                                 65536,
                },
                { NULL, NULL, 0, 0 } /* terminator */
        };



int main(void)
{

    struct lws_context *context = NULL;

    struct lws_context_creation_info info;
    memset( &info, 0, sizeof(info) );

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    info.iface = NULL;
    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;
    info.extensions = NULL;
    info.gid = -1;
    info.uid = -1;
    info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    info.max_http_header_pool = 1024;
    info.fd_limit_per_thread = 1024;
    info.ws_ping_pong_interval = 10;
    info.ka_time = 10;
    info.ka_probes = 10;
    info.ka_interval = 10;

    context = lws_create_context( &info );
    cout << "[Main] context created...." << endl;
    if (context == NULL) {
        cout << "[Main] context is NULL." << endl;
        return -1;
    }

    //std::string path("/api/tradeview/ETH-BTC");

    int logs = LLL_ERR | LLL_DEBUG | LLL_WARN;
    lws_set_log_level(logs, NULL);

    struct lws_client_connect_info ccinfo = {0};


    ccinfo.context 	= context;
    ccinfo.address 	= host.c_str();
    ccinfo.port 	= port;
    ccinfo.path 	= path.c_str();
    ccinfo.host 	= host.c_str();
    ccinfo.origin 	= host.c_str();
    ccinfo.ietf_version_or_minus_one = -1;
    ccinfo.protocol = protocols[0].name;
    ccinfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;


    struct lws* wsi = lws_client_connect_via_info(&ccinfo);

    cout << "Connecting to " <<  ccinfo.host << ":" << ccinfo.port << ":" << ccinfo.path << endl;

//    clientConnectInfo.pwsi = &wsi;


    if (wsi == NULL) {
        cout << "[Main] wsi create error." << endl;
        return -1;
    }

    cout << "[Main] wsi create success." << endl;

    lws_callback_on_writable(wsi);

    while(true)
    {
        lws_service(context, 500);
    }

    lws_context_destroy(context);

    return 0;
}
