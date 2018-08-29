/*
 * $Id: $
 *
 * see http://stackoverflow.com/questions/669438/how-to-get-memory-usage-at-run-time-in-c
 *
 * Copyright (c) 2014-2015 Zerone.IO (Shanghai). All rights reserved.
 *
 * $Log: $
 *
 */
#pragma once

/**
 * Returns the peak (maximum so far) resident set size (physical
 * memory use) measured in bytes, or zero if the value cannot be
 * determined on this OS.
 */
size_t getPeakRSS();

/**
 * Returns the current resident set size (physical memory use) measured
 * in bytes, or zero if the value cannot be determined on this OS.
 */
size_t getCurrentRSS();

/**
 * process_mem_usage(double &, double &) - takes two doubles by reference,
 * attempts to read the system-dependent data for a process' virtual memory
 * size and resident set size, and return the results in KB.
 * On failure, returns 0.0, 0.0
 */
void process_mem_usage(double &vm_usage, double &resident_set);

void print_all_usage(bool reset = false);
