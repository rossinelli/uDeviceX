/*
 *  2Dto3D.h
 *  Part of CTC/device-gen/common/
 *
 *  Created and authored by Diego Rossinelli and Kirill Lykov on 2015-03-20.
 *  Copyright 2015. All rights reserved.
 *
 */
#pragma once
#include <vector>
#include <string>

void conver2Dto3D(const int NX, const int NY, const float xextent, const float yextent, const std::vector<float>& slice,
                  const int NZ, const float zextent, const float zmargin, const std::string& fileName);
