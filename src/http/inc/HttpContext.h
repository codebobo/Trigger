
#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <StringBuffer.h>
#include "TrantorTimestamp.h"

class HttpContext
{
 public:
  enum HttpRequestParseState
  {
    kExpectRequestLine,
    kExpectHeaders,
    kExpectBody,
    kGotAll,
  };

  enum class HttpResponseParseState
  {
    kExpectResponseLine,
    kExpectHeaders,
    kExpectBody,
    kExpectChunkLen,
    kExpectChunkBody,
    kExpectLastEmptyChunk,
    kExpectClose,
    kGotAll,
  };

  HttpContext()
    : state_(kExpectRequestLine),
    res_state_(HttpResponseParseState::kExpectResponseLine)
  {
  }

  // default copy-ctor, dtor and assignment are fine

  // return false if any error
  bool parseRequest(StringBuffer* buf, TrantorTimestamp receiveTime);
  bool parseResponse(StringBuffer* buf, TrantorTimestamp receiveTime);

  bool gotAll() const
  { return state_ == kGotAll; }

  bool resGotAll() const
  { return res_state_ == HttpResponseParseState::kGotAll; }

  bool resExpectResponseLine() const
  { return res_state_ == HttpResponseParseState::kExpectResponseLine; }

  void reset()
  {
    state_ = kExpectRequestLine;
    HttpRequest dummy;
    request_.swap(dummy);
  }

  void resetRes()
  {
    res_state_ = HttpResponseParseState::kExpectResponseLine;
    response_.clear();
  }

  const HttpRequest& request() const
  { return request_; }

  HttpRequest& request()
  { return request_; }

  const HttpResponse& response() const
  { return response_; }

  HttpResponse& response()
  { return response_; }

 private:
  bool processRequestLine(const char* begin, const char* end);
  bool processResponseLine(const char* begin, const char* end);

  HttpRequestParseState state_;
  HttpRequest request_;

  HttpResponseParseState res_state_;
  HttpResponse response_;
};


#endif  
