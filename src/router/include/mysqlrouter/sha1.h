/* Copyright (c) 2002, 2017, Oracle and/or its affiliates. All rights reserved.

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef SHA1_INCLUDED
#define SHA1_INCLUDED

#include <cstddef>
#include <cstdint>

#define SHA1_HASH_SIZE 20 /* Hash size in bytes */

namespace my_sha1 {

void compute_sha1_hash(uint8_t *digest, const char *buf, size_t len);
void compute_sha1_hash_multi(uint8_t *digest, const char *buf1, int len1,
                             const char *buf2, int len2);

}

#endif /* SHA1_INCLUDED */
