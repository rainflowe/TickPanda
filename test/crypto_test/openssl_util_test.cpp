#include "../../utils/crypto/openssl_util.h"

#include <assert.h>
#include <iostream>

// example in https://github.com/binance-exchange/binance-official-api-docs/blob/master/rest-api.md

int main() 
{
  const std::string secret = "NhqPtmdSJYdKjVHjA7PZj4Mge3R5YNiP1e3UZjInClVN65XAbvqqM6A7H5fATj0j";
  const std::string message = "symbol=LTCBTC&side=BUY&type=LIMIT&timeInForce=GTC&quantity=1&price=0.1&recvWindow=5000&timestamp=1499827319559";

  const std::string signature = utils::crypto::hmac_sha256(secret.c_str(), message.c_str());
  assert(signature == "c8db56825ae71d6d79447849e617115f4a920fa2acdcab2b053c4b2838bd6b71");
  std::cout << "signature is: " << signature << std::endl;
  return 0;
}
