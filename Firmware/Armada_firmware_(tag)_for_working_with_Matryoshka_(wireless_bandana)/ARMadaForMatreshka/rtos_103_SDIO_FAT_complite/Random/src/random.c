
/*
*    Copyright (C) 2016 by Aleksey Bulatov
*
*    This file is part of Caustic Lasertag System project.
*
*    Caustic Lasertag System is free software:
*    you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    Caustic Lasertag System is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with Caustic Lasertag System.
*    If not, see <http://www.gnu.org/licenses/>.
*
*    @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
*/


#include "random.h"

#include <stdio.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"

static uint32_t m_lastSeed = 0;


uint16_t random16(void){
	return 0xFFFF & random32();
 }
 uint32_t random32(void){
	 m_lastSeed = 0xFFFFFFFF & ( (m_lastSeed+1) * ( xTaskGetTickCount() & 0xFFFF));
	 return m_lastSeed;

 }
 uint32_t random(uint32_t maxValue){
	 return (uint32_t) round((( (float) random32()) / UINT32_MAX * maxValue));
 }
