/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#ifndef __FRC_ROBORIO__

#include "MockData/MockHooks.h"

namespace frc {
namespace sim {
void WaitForProgramStart() { HALSIM_WaitForProgramStart(); }
void SetProgramStarted() { HALSIM_SetProgramStarted(); }
void RestartTiming() { HALSIM_RestartTiming(); }
}  // namespace sim
}  // namespace frc

#endif
