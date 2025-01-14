// Copyright 2023 The SOLA authors
//
// This file is part of DAISI.
//
// DAISI is free software: you can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation; version 2.
//
// DAISI is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with DAISI. If not, see
// <https://www.gnu.org/licenses/>.
//
// SPDX-License-Identifier: GPL-2.0-only

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "cpps/common/cpps_manager.h"
#include "minhton-ns3/minhton_manager.h"
#include "natter-ns3/natter_manager.h"
#include "ns3/core-module.h"
// #include "path_planning/path_planning_manager.h"
#include "sola-ns3/sola_manager.h"
#include "utils/sola_utils.h"

using namespace ns3;

template <typename Manager> int runSimulatorWithCatch(Manager &manager) {
  int res = EXIT_SUCCESS;
  try {
    manager.run();
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    std::cerr << "Logging to database..." << std::endl;
    manager.markAsFailed(ex.what());
    std::cerr << "Aborting..." << std::endl;
    res = EXIT_FAILURE;
  }
  return res;
}

template <typename Manager> int execute(std::string test_file, bool disable_catch) {
  std::cout << "Executing " << typeid(Manager).name() << std::endl;
  Manager manager(test_file);
  manager.setup();

  if (disable_catch) {
    manager.run();
    // If we get here, no exception was thrown
    return EXIT_SUCCESS;
  }

  return runSimulatorWithCatch(manager);
}

int main(int argc, char *argv[]) {
  auto start_time = std::chrono::high_resolution_clock::now();

  std::string param_environment;
  std::string param_scenariofile;
  std::string param_scenariostring;
  bool disable_catch = false;

  CommandLine cmd;
  cmd.AddValue("environment", "test environment to run - (minhton, natter, cpps, path_planning)",
               param_environment);
  cmd.AddValue("scenario", "scenario file to run", param_scenariofile);
  cmd.AddValue("scenariostring", "test string to run", param_scenariostring);
  cmd.AddValue("disable-catch", "disable catching fatal errors (for debugging)", disable_catch);
  cmd.Parse(argc, argv);

  std::cout << "NS-3 Simulation " << std::endl;

  std::string test_file;
  int ret = daisi::getTestFile(param_scenariofile, param_scenariostring, test_file);

  if (ret != 0) {
    std::cout << "error" << std::endl;
    return 0;
  }

  Config::SetDefault("ns3::ArpCache::PendingQueueSize", UintegerValue(UINT32_MAX));

  std::cout << "Starting simulation with scenariofile: " << std::endl << test_file << std::endl;
  std::time_t result = std::time(nullptr);
  std::cout << "StartTime: " << std::ctime(&result);

  int res = EXIT_SUCCESS;

  if (param_environment == "minhton") {
    res = execute<daisi::minhton_ns3::MinhtonManager>(test_file, disable_catch);
  } else if (param_environment == "natter") {
    res = execute<daisi::natter_ns3::NatterManager>(test_file, disable_catch);
  } else if (param_environment == "cpps") {
    res = execute<daisi::cpps::CppsManager>(test_file, disable_catch);
    // } else if (param_environment == "path_planning") {
    //   res = execute<daisi::path_planning::PathPlanningManager>(test_file, disable_catch);
  } else if (param_environment == "sola") {
    res = execute<daisi::sola_ns3::SolaManager>(test_file, disable_catch);
  } else {
    std::cerr << "Invalid environment! Terminating..." << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Simulation Destroy" << std::endl;
  result = std::time(nullptr);
  std::cout << "StopTime: " << std::ctime(&result) << std::endl;

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> exec_time = end_time - start_time;
  std::cout << "Execution time: " << exec_time.count() << " seconds" << std::endl;
  return res;
}
