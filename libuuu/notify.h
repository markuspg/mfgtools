/*
 * Copyright 2021 NXP.
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


#pragma once

#include <cstddef>
#include <cstdint>

/**
 * @brief Data structure for storing information that shall be sent as
 * notification to all registered subscribers
 */
struct uuu_notify
{
	enum class NOTIFCTN_TYPE : uint_fast8_t
	{
		//! The total number of commands being executed (`total` is set to the
		//! number of commands)
		CMD_TOTAL,
		//! A command started execution (`str` is set to the command's name)
		CMD_START,
		//! Command execution ended (`status` is set to `0` on success, any
		//! other value on failure)
		CMD_END,
		//! Index of a command in a command list about to be executed (`index`
		//! is set accordingly)
		CMD_INDEX,

		//! Textual information from a command (`str` contains the information)
		CMD_INFO,

		//! Unused
		PHASE_TOTAL,
		//! Unused
		PHASE_INDEX,

		//! The entire size of a transfer (`total` is set to the total size)
		TRANS_SIZE,
		//! The current offset/progress of the transfer (`index` or `total` are
		//! set to the current progress)
		TRANS_POS,

		//! A USB device is being waited for (`str` is set to a descriptive
		//! message)
		WAIT_FOR,
		//! A USB device has been attached (`str` contains the device's path)
		DEV_ATTACH,

		//! Decompression of a file started (`str` is set to the compressed
		//! file's name)
		DECOMPRESS_START,
		//! Size of the file being decompressed (`total` is set to the size)
		DECOMPRESS_SIZE,
		//! The position in the file being decompressed (`index` is set to the
		//! position)
		DECOMPRESS_POS,

		//! A HTTP download started (`str` is set to the host)
		DOWNLOAD_START,
		//! A HTTP download ended (`str` is set to the host)
		DOWNLOAD_END,
		//! A thread for the execution of USB commands exited (nothing is set)
		THREAD_EXIT,

		//! The _Done_ command runs (`nothing` is set)
		DONE,
	};

	//! The type of the notification
	NOTIFCTN_TYPE type;
	//! The id of the thread sending the notification
	uint64_t id;
	//! Timestamp of the notification submission time (mostly concides with its
	//! creation time)
	uint64_t timestamp;
	union
	{
		int status;
		size_t index;
		size_t total;
		char *str;
	};
};

/**
 * @brief Method which is to be called upon notifications
 * @param[in] notifctn The notification which got send
 * @param[in] data Optional additional data
 */
using uuu_notify_fun = int (*)(struct uuu_notify notifctn, void *data);
void call_notify(struct uuu_notify nf);
int uuu_register_notify_callback(uuu_notify_fun f, void *data);
int uuu_unregister_notify_callback(uuu_notify_fun f);
