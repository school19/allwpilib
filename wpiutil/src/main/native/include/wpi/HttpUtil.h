/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef WPIUTIL_SUPPORT_HTTPUTIL_H_
#define WPIUTIL_SUPPORT_HTTPUTIL_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "wpi/ArrayRef.h"
#include "wpi/NetworkStream.h"
#include "wpi/SmallString.h"
#include "wpi/SmallVector.h"
#include "wpi/StringMap.h"
#include "wpi/StringRef.h"
#include "wpi/Twine.h"
#include "wpi/raw_istream.h"
#include "wpi/raw_socket_istream.h"
#include "wpi/raw_socket_ostream.h"

namespace wpi {

// Unescape a %xx-encoded URI.
// @param buf Buffer for output
// @param error Set to true if an error occurred
// @return Escaped string
StringRef UnescapeURI(const Twine& str, SmallVectorImpl<char>& buf,
                      bool* error);

// Escape a string with %xx-encoding.
// @param buf Buffer for output
// @param spacePlus If true, encodes spaces to '+' rather than "%20"
// @return Escaped string
StringRef EscapeURI(const Twine& str, SmallVectorImpl<char>& buf,
                    bool spacePlus = true);

// Parse a set of HTTP headers.  Saves just the Content-Type and Content-Length
// fields.
// @param is Input stream
// @param contentType If not null, Content-Type contents are saved here.
// @param contentLength If not null, Content-Length contents are saved here.
// @return False if error occurred in input stream
bool ParseHttpHeaders(raw_istream& is, SmallVectorImpl<char>* contentType,
                      SmallVectorImpl<char>* contentLength);

// Look for a MIME multi-part boundary.  On return, the input stream will
// be located at the character following the boundary (usually "\r\n").
// @param is Input stream
// @param boundary Boundary string to scan for (not including "--" prefix)
// @param saveBuf If not null, all scanned characters up to but not including
//     the boundary are saved to this string
// @return False if error occurred on input stream, true if boundary found.
bool FindMultipartBoundary(wpi::raw_istream& is, StringRef boundary,
                           std::string* saveBuf);

class HttpLocation {
 public:
  HttpLocation() = default;
  HttpLocation(const Twine& url_, bool* error, std::string* errorMsg);

  std::string url;       // retain copy
  std::string user;      // unescaped
  std::string password;  // unescaped
  std::string host;
  int port;
  std::string path;  // escaped, not including leading '/'
  std::vector<std::pair<std::string, std::string>> params;  // unescaped
  std::string fragment;
};

class HttpRequest {
 public:
  HttpRequest() = default;

  explicit HttpRequest(const HttpLocation& loc)
      : host{loc.host}, port{loc.port} {
    SetPath(loc.path, loc.params);
    SetAuth(loc);
  }

  template <typename T>
  HttpRequest(const HttpLocation& loc, const T& extraParams);

  HttpRequest(const HttpLocation& loc, StringRef path_)
      : host{loc.host}, port{loc.port}, path{path_} {
    SetAuth(loc);
  }

  template <typename T>
  HttpRequest(const HttpLocation& loc, StringRef path_, const T& params)
      : host{loc.host}, port{loc.port} {
    SetPath(path_, params);
    SetAuth(loc);
  }

  SmallString<128> host;
  int port;
  std::string auth;
  SmallString<128> path;

 private:
  void SetAuth(const HttpLocation& loc);
  template <typename T>
  void SetPath(StringRef path_, const T& params);

  template <typename T>
  static StringRef GetFirst(const T& elem) {
    return elem.first;
  }
  template <typename T>
  static StringRef GetFirst(const StringMapEntry<T>& elem) {
    return elem.getKey();
  }
  template <typename T>
  static StringRef GetSecond(const T& elem) {
    return elem.second;
  }
};

class HttpConnection {
 public:
  HttpConnection(std::unique_ptr<wpi::NetworkStream> stream_, int timeout)
      : stream{std::move(stream_)}, is{*stream, timeout}, os{*stream, true} {}

  bool Handshake(const HttpRequest& request, std::string* warnMsg);

  std::unique_ptr<wpi::NetworkStream> stream;
  wpi::raw_socket_istream is;
  wpi::raw_socket_ostream os;

  // Valid after Handshake() is successful
  SmallString<64> contentType;
  SmallString<64> contentLength;

  explicit operator bool() const { return stream && !is.has_error(); }
};

}  // namespace wpi

#include "HttpUtil.inl"

#endif  // WPIUTIL_SUPPORT_HTTPUTIL_H_
