#include <iostream>
#include <cstdlib>
#include <string>
#include <signal.h>

#include <document.h>
#include <writer.h>
#include <stringbuffer.h>

using namespace rapidjson;
using namespace std;

#include <libwebsockets.h>

static int destroy_flag = 0;
static int writeable_flag = 0;

struct session_data {
    int fd;
};

static int ws_service_cb(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);

static void
sigint_handler(int sig)
{
	destroy_flag = 1;
}

static struct lws_protocols protocols[] = {
	{
		"test-protocol",
		ws_service_cb,
		0,
		1024,
		
	},
	{NULL, NULL, 0}
};

enum protocolList {
	PROTOCOL_TEST,

	PROTOCOL_LIST_COUNT
};

enum RequestType {
        Depth,
	Order,
	Ticker,
	Signin,
	Asset
};

std::string createJsonString(RequestType t)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	
   	switch (t)
	{
	   case Depth:
		writer.StartObject();
		writer.Key("event");
		writer.String("subscribe");
		writer.Key("params");
		writer.StartObject();
		writer.Key("biz");
		writer.String("spot");
		writer.Key("type");
		writer.String("depth");
		writer.Key("base");
		writer.String("btc");
		writer.Key("quote");
		writer.String("usdt");
		writer.Key("zip");
		writer.Bool(false);
		writer.EndObject();
		writer.EndObject();
		break;

	   case Ticker:
		writer.StartObject();
		writer.Key("event");
		writer.String("subscribe");
		writer.Key("params");
		writer.StartObject();
		writer.Key("biz");
		writer.String("spot");
		writer.Key("type");
		writer.String("tickers");
		writer.Key("zip");
		writer.Bool(false);
		writer.EndObject();
		writer.EndObject();
		
		break;

	  case Order:
		break;

	  default:
		break;
	}

	return s.GetString();
}

std::string getJson(void* in)
{
	Document d;
	d.Parse(reinterpret_cast<const char*>(in));

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	return buffer.GetString();
}

static int ws_service_cb(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
	unsigned char msg[512]; 
	int ret = 0;
	int length = 0;
	std::string tmpString;
	std::string jsonString;

	memset(&msg[LWS_PRE],0,512-LWS_PRE);

	switch (reason) {

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		cout << "callback client established, reason = " << reason << endl;
		lws_callback_on_writable(wsi);
		return 0;

	case LWS_CALLBACK_PROTOCOL_INIT:
		cout << "init, reason = " << reason << endl;
		return 0;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		cout << "writeable, reason = " << reason << endl;
		// send json request

		jsonString = createJsonString(RequestType::Depth);
		lwsl_debug((char *)(jsonString.c_str()));
		length = jsonString.length();

		strncpy((char *)msg+LWS_PRE, jsonString.c_str(), length);
		lwsl_debug(reinterpret_cast<char *>(msg+LWS_PRE));
		ret = lws_write(wsi, &msg[LWS_PRE], length,LWS_WRITE_TEXT);
		return 0;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		//lwsl_debug("incoming data: ");
		//cout << reinterpret_cast<char *>(in) << endl;
		lwsl_debug((char *)(getJson(in).c_str()));
		cout << getJson(in) << endl;
		return 0;

	default:
		return 0;
	}

	return 0;
}

int main(void)
{
	
    char inputURL[300] = "wss://websocket.coinmex.com";
    int inputPort = 8443;
    const char *urlProtocol, *urlTempPath;
    char urlPath[300];
    int logs = LLL_ERR | LLL_DEBUG | LLL_WARN;

    signal(SIGINT, sigint_handler);
    lws_set_log_level(logs, NULL);

    struct lws_context *context = NULL;
    struct lws_context_creation_info ctxCreationInfo;
    struct lws_client_connect_info clientConnectInfo;
    struct lws *wsi = NULL;
    struct lws_protocols protocol;

    memset(&ctxCreationInfo, 0, sizeof(ctxCreationInfo));
    memset(&clientConnectInfo, 0, sizeof(clientConnectInfo));

    clientConnectInfo.port = 8443;

    if (lws_parse_uri(inputURL, &urlProtocol, &clientConnectInfo.address, &clientConnectInfo.port, &urlTempPath))
    {
        cout << "Couldn't parse URL." << endl;
    }

    // Fix up the urlPath by adding a / at the beginning, copy the temp path, and add a \0     at the end
    urlPath[0] = '/';
    strncpy(urlPath + 1, urlTempPath, sizeof(urlPath) - 2);
    urlPath[sizeof(urlPath) - 1] = '\0';
    clientConnectInfo.path = urlPath; // Set the info's path to the fixed up url path

    cout << "urlProtocol=" << urlProtocol << endl;
    cout << "address=" << clientConnectInfo.address << endl;
    cout << "urlTempPath=" << urlTempPath << endl;
    cout << "urlPath=" << urlPath << endl ;

  
    ctxCreationInfo.port = CONTEXT_PORT_NO_LISTEN;
    ctxCreationInfo.iface = NULL;
    ctxCreationInfo.protocols = &protocol;
    ctxCreationInfo.ssl_cert_filepath = NULL;
    ctxCreationInfo.ssl_private_key_filepath = NULL;
    ctxCreationInfo.extensions = NULL;
    ctxCreationInfo.gid = -1;
    ctxCreationInfo.uid = -1;
    ctxCreationInfo.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
    ctxCreationInfo.fd_limit_per_thread = 1024;
    ctxCreationInfo.max_http_header_pool = 1024;
    ctxCreationInfo.ws_ping_pong_interval = 1;

    protocol.name  = protocols[PROTOCOL_TEST].name;
    protocol.callback = &ws_service_cb;
    protocol.per_session_data_size = sizeof(struct session_data);
    protocol.rx_buffer_size = 0;
    protocol.id = 0;
    protocol.user = NULL;

    context = lws_create_context(&ctxCreationInfo);
    cout << "[Main] context created." << endl;

    if (context == NULL) {
        cout << "[Main] context is NULL." << endl;
        return -1;
    }

    // Set up the client creation info
    clientConnectInfo.context = context;
    clientConnectInfo.port = 8443;
    clientConnectInfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
    clientConnectInfo.host = clientConnectInfo.address;
    clientConnectInfo.origin = clientConnectInfo.address;
    clientConnectInfo.ietf_version_or_minus_one = -1;
    clientConnectInfo.protocol = protocols[PROTOCOL_TEST].name;
    clientConnectInfo.pwsi = &wsi;
 
    cout << "Connecting to " << urlProtocol << ":" << clientConnectInfo.host << ":" << clientConnectInfo.port << ":" << urlPath << endl;

    wsi = lws_client_connect_via_info(&clientConnectInfo);
    if (wsi == NULL) {
        cout << "[Main] wsi create error." << endl;
        return -1;
    }

    cout << "[Main] wsi create success." << endl;

    lws_callback_on_writable(wsi);

    while(!destroy_flag)
    {
        lws_service(context, 500);
    }

    lws_context_destroy(context);

    return 0;
}
