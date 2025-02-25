#if defined _INC_y_inline_timers
	#endinput
#endif
#define _INC_y_inline_timers

/*
Legal:
	Version: MPL 1.1
	
	The contents of this file are subject to the Mozilla Public License Version 
	1.1 the "License"; you may not use this file except in compliance with 
	the License. You may obtain a copy of the License at 
	http://www.mozilla.org/MPL/
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the YSI framework.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright (c) 2022
	the Initial Developer. All Rights Reserved.

Contributors:
	Y_Less
	koolk
	JoeBullet/Google63
	g_aSlice/Slice
	Misiur
	samphunter
	tianmeta
	maddinat0r
	spacemud
	Crayder
	Dayvison
	Ahmad45123
	Zeex
	irinel1996
	Yiin-
	Chaprnks
	Konstantinos
	Masterchen09
	Southclaws
	PatchwerkQWER
	m0k1
	paulommu
	udan111
	Cheaterman

Thanks:
	JoeBullet/Google63 - Handy arbitrary ASM jump code using SCTRL.
	ZeeX - Very productive conversations.
	koolk - IsPlayerinAreaEx code.
	TheAlpha - Danish translation.
	breadfish - German translation.
	Fireburn - Dutch translation.
	yom - French translation.
	50p - Polish translation.
	Zamaroht - Spanish translation.
	Los - Portuguese translation.
	Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes for
		me to strive to better.
	Pixels^ - Running XScripters where the idea was born.
	Matite - Pestering me to release it and using it.

Very special thanks to:
	Thiadmer - PAWN, whose limits continue to amaze me!
	Kye/Kalcor - SA:MP.
	SA:MP Team past, present and future - SA:MP.

Optional plugins:
	Gamer_Z - GPS.
	Incognito - Streamer.
	Me - sscanf2, fixes2, Whirlpool.
*/

#if !defined _INC_y_inline
	#tryinclude "..\YSI_Coding\y_inline\y_inline_entry"
#endif
#if !defined _INC_y_inline
	#tryinclude "YSI_Coding\y_inline\y_inline_entry"
#endif
#if !defined _INC_y_inline
	#tryinclude "YSI\YSI_Coding\y_inline\y_inline_entry"
#endif
#if !defined _INC_y_inline
	#tryinclude <YSI_Coding\y_inline\y_inline_entry>
#endif
#if !defined _INC_y_inline
	#tryinclude <YSI\YSI_Coding\y_inline\y_inline_entry>
#endif
#if !defined _INC_y_inline
	#error Could not find y_inline
#endif

/*-------------------------------------------------------------------------*//**
 * <library>y_inline_timers</library>
 *//*------------------------------------------------------------------------**/

static stock
	YSI_gsCalls = 0;

/*-------------------------------------------------------------------------*//**
 * <library>y_inline_timers</library>
 * <remarks>
 *   Internal function to just be called repeatedly.
 * </remarks>
 *//*------------------------------------------------------------------------**/

static stock Timer_InlineRepeat(Func:func<>)
{
	Debug_Print5("Timer_InlineRepeat called: %d", _:func);
	// Normal repeating timer.
	@.func();
}

/*-------------------------------------------------------------------------*//**
 * <library>y_inline_timers</library>
 * <remarks>
 *   Internal function to allow initial offset times.
 * </remarks>
 *//*------------------------------------------------------------------------**/

static stock Timer_InlineCount(Func:func<>, time, count)
{
	Debug_Print5("Timer_InlineCount called: %d, %d, %d", _:func, time, count);
	switch (--count)
	{
	case -1:
	{
		// Initial repeating timer, to set the time offset.
		//printf("delay = %d", timer);
		Indirect_SetMeta(func, SetTimerEx(&Timer_InlineRepeat<tF@_@:>, time, true, YSI_gcI, _:func));
		// Call the function after setting the meta, incase it kills the timer.
		@.func();
	}
	case 0:
	{
		// Call before release.
		@.func();
		Indirect_Release(func);
		Debug_Print7("y_inlineTimerCount ending: %d (%d)", _:func, ++YSI_gsCalls);
	}
	default:
	{
		// Call after meta.
		Indirect_SetMeta(func, SetTimerEx(&Timer_InlineCount<tF@_@:ii>, time, false, YSI_gcIII, _:func, time, count));
		@.func();
	}
	}
}

/*-------------------------------------------------------------------------*//**
 * <library>y_inline_timers</library>
 * <param name="func">Function to call on a timer.</param>
 * <param name="initial">Delay before first call.</param>
 * <param name="time">Delay between subsequent calls.</param>
 * <param name="count">How many times to call it.</param>
 * <remarks>
 *   There are several ways to use this function:
 *  
 *   <c>SetCallbackTimer(func, 100);</c>
 *   Calls the function once every 100ms.
 *  
 *   <c>SetCallbackTimer(func, 100, 0);</c>
 *   Same as above (count is <c>0</c>, which means repeat forever).
 *  
 *   <c>SetCallbackTimer(func, 100, 5);</c>
 *   Calls the function once every 100ms, but only 5 times.
 *  
 *   <c>SetCallbackTimer(func, 100, 200, 0);</c>
 *   Calls the function once every 200ms, but the first call is after just 100ms.
 *  
 *   <c>SetCallbackTimer(func, 100, 200, 42);</c>
 *   Calls the function 42 times, first after 100ms, then every 200ms.
 *  
 *   When only one parameter is given its <c>time</c>.
 *  
 *   When two parameters are given they're <c>time</c> and <c>count</c>.
 *  
 *   When all three parameters are given they're <c>initial</c>, <c>time</c>,
 *   and <c>count</c>.
 *  
 *   Count is NOT like <c>repeat</c> in <c>SetTimer</c>.  <c>0</c> means repeat
 *   forever, anything not <c>0</c> means call it exactly that many times.
 * </remarks>
 *//*------------------------------------------------------------------------**/

stock Timer_CreateCallback(Func:func<>, initialOrTime, timeOrCount = 0, count = -1)
{
	Debug_Print3("Timer_CreateCallback called: %d, %d, %d, %d", _:func, initialOrTime, timeOrCount, count);
	if (count == -1)
	{
		// No `initial` time.
		count = timeOrCount;
		timeOrCount = initialOrTime;
	}
	if (count < 0)
	{
		return 0;
	}
	// `0` means "repeat forever", but DON'T repeat here, this is just to set
	// the initial offset time.
	// Repeat N times.
	Indirect_Claim(func);
	Indirect_SetMeta(func, SetTimerEx(&Timer_InlineCount<tF@_@:ii>, initialOrTime, false, YSI_gcIII, _:func, timeOrCount, count));
	return _:func;
}

#if __COMPILER_WARNING_SUPPRESSION
	// On old compilers, only issue the warning at the call site.
	#pragma warning push
	#pragma warning disable 234
#endif

/*-------------------------------------------------------------------------*//**
 * <library>y_inline_timers</library>
 * <remarks>
 *   Old way to create an inline timer.
 * </remarks>
 *//*------------------------------------------------------------------------**/

#pragma deprecated Use `Timer_CreateCallback`.
stock SetCallbackTimer(Func:func<>, initialOrTime, timeOrCount = 0, count = -1)
{
	return Timer_CreateCallback(func, initialOrTime, timeOrCount, count);
}

#if __COMPILER_WARNING_SUPPRESSION
	#pragma warning pop
#endif

/*-------------------------------------------------------------------------*//**
 * <library>y_inline_timers</library>
 * <param name="func">Timer to kill.</param>
 * <remarks>
 *   Kill a timer created by <c>SetCallbackTimer</c>.  Normal <c>KillTimer</c>
 *   won't work for them.
 * </remarks>
 *//*------------------------------------------------------------------------**/

stock Timer_KillCallback(func)
{
	Debug_Print3("Timer_KillCallback called: %d", func);
	new timer = Indirect_GetMeta(func);
	Indirect_Release(func);
	return KillTimer(timer);
}

#if __COMPILER_WARNING_SUPPRESSION
	// On old compilers, only issue the warning at the call site.
	#pragma warning push
	#pragma warning disable 234
#endif

/*-------------------------------------------------------------------------*//**
 * <library>y_inline_timers</library>
 * <param name="func">Timer to kill.</param>
 * <remarks>
 *   Kill a timer created by <c>SetCallbackTimer</c>.  Normal <c>KillTimer</c>
 *   won't work for them.
 * </remarks>
 *//*------------------------------------------------------------------------**/

#pragma deprecated Use `Timer_KillCallback`.
stock KillCallbackTimer(func)
{
	return Timer_KillCallback(func);
}

#if __COMPILER_WARNING_SUPPRESSION
	#pragma warning pop
#endif

