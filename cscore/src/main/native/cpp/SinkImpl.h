/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_SINKIMPL_H_
#define CSCORE_SINKIMPL_H_

#include <memory>
#include <string>

#include <wpi/StringRef.h>
#include <wpi/mutex.h>

#include "SourceImpl.h"

namespace cs {

class Frame;

class SinkImpl {
 public:
  explicit SinkImpl(wpi::StringRef name);
  virtual ~SinkImpl();
  SinkImpl(const SinkImpl& queue) = delete;
  SinkImpl& operator=(const SinkImpl& queue) = delete;

  wpi::StringRef GetName() const { return m_name; }

  void SetDescription(wpi::StringRef description);
  wpi::StringRef GetDescription(wpi::SmallVectorImpl<char>& buf) const;

  void Enable();
  void Disable();
  void SetEnabled(bool enabled);

  void SetSource(std::shared_ptr<SourceImpl> source);

  std::shared_ptr<SourceImpl> GetSource() const {
    std::lock_guard<wpi::mutex> lock(m_mutex);
    return m_source;
  }

  std::string GetError() const;
  wpi::StringRef GetError(wpi::SmallVectorImpl<char>& buf) const;

 protected:
  virtual void SetSourceImpl(std::shared_ptr<SourceImpl> source);

  mutable wpi::mutex m_mutex;

 private:
  std::string m_name;
  std::string m_description;
  std::shared_ptr<SourceImpl> m_source;
  int m_enabledCount{0};
};

}  // namespace cs

#endif  // CSCORE_SINKIMPL_H_
