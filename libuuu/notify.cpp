/*
* Copyright 2018 NXP.
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice, this
* list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* Neither the name of the NXP Semiconductor nor the names of its
* contributors may be used to endorse or promote products derived from this
* software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "notify.h"
#include "libuuu.h"

#include <chrono>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>

using namespace std;

static map<uuu_notify_fun, void*> g_notification_map;
static mutex g_mutex_notify;

using namespace std::chrono;
static const time_point<steady_clock> g_now = steady_clock::now();

/**
 * @brief Register a new callback function which shall be called upon a
 * notification
 * @param[in] f The function which shall be called upon a notification
 * @param[in] data Pointer to data which shall be passed to the callback method
 * @return `0` if the callback function got registered, `1` if it already is
 */
int uuu_register_notify_callback(uuu_notify_fun f, void *data)
{
	std::lock_guard<mutex> lock(g_mutex_notify);

	return g_notification_map.emplace(f, data).second ? 0 : 1;
}

/**
 * @brief Unregister a callback function
 * @param[in] f The function which shall not be called upon a notification
 * anymore
 * @return `0` if it got removed, `1` otherwise
 */
int uuu_unregister_notify_callback(uuu_notify_fun f)
{
	std::lock_guard<mutex> lock(g_mutex_notify);

	return g_notification_map.erase(f) > 0 ? 0 : 1;
}

/**
 * @brief Submit a notification to all registered callbacks
 * @param[in] nf The notification which shall be submitted to the callbacks
 */
void call_notify(struct uuu_notify nf)
{
	//Change RW lock later;
	std::lock_guard<mutex> lock(g_mutex_notify);

	nf.id = std::hash<std::thread::id>{}(std::this_thread::get_id());
	nf.timestamp = static_cast<uint64_t>(
		duration_cast<milliseconds>(steady_clock::now() - g_now).count());

	for (const auto &item : g_notification_map)
	{
		try {
			item.first(nf, item.second);
		} catch (const std::exception& e) {
			std::cerr << "notify exception: " << e.what() << std::endl;
		}
	}
}
