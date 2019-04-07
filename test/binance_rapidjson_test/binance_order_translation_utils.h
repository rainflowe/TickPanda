#include "../../longfist/longfist/LFDataStruct.h"
#include "../../longfist/longfist/LFConstants.h"

#include <string>
#include <iostream>

std::string GetSide(const LfDirectionType& input) {
  if (LF_CHAR_Buy == input) {
    return "BUY";
  } else if (LF_CHAR_Sell == input) {
    return "SELL";
  } else {
    return "UNKNOWN";
  }
}

std::string GetType(const LfOrderPriceTypeType& input) {
  if (LF_CHAR_LimitPrice == input) {
    return "LIMIT";
  } else if (LF_CHAR_AnyPrice == input) {
    return "MARKET";
  } else {
    return "UNKNOWN";
  }
}

std::string GetTimeInForce(const LfTimeConditionType& input) {
  if (LF_CHAR_IOC == input) {
    return "IOC";
  } else if (LF_CHAR_GTC == input) {
    return "GTC";
  } else if (LF_CHAR_FOK == input) {
    return "FOK";
  } else {
    return "UNKNOWN";
  }
}
