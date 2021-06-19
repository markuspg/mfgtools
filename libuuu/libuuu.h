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
#ifndef __libuuu___
#define __libuuu___

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
#define EXT extern "C"
#else
#define EXT
#endif

/**
 * Get Last error string
 * @return last error string
*/
EXT const char * uuu_get_last_err_string();

/**
* Get Last error code
* @return last error code
*/
EXT int uuu_get_last_err();

EXT const char * uuu_get_version_string();

/**
 * 1.0.1
 * bit[31:24].bit[23:12].bit[11:0]
 */

EXT int uuu_get_version();

typedef int(*uuu_show_cfg)(const char *pro, const char *chip, const char *comp, uint16_t vid, uint16_t pid, uint16_t bcdlow, uint16_t bcdhigh, void *p);
int uuu_for_each_cfg(uuu_show_cfg fn, void *p);

typedef int(*uuu_ls_file)(const char *path, void *p);
int uuu_for_each_ls_file(uuu_ls_file fn, const char *path, void *p);

int uuu_run_cmd(const char * cmd, int dry);
int uuu_run_cmd_script(const char *script, int dry);

int uuu_auto_detect_file(const char * filename);
int uuu_wait_uuu_finish(int deamon, int dry);

/*
 * bit 0:15 for libusb
 * bit 16:31 for uuu
 */
void uuu_set_debug_level(uint32_t mask);

#endif
