/*
 * MIT License
 *
 * Copyright (c) 2022 WeCom-Open
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "file_block_digest.h"

using namespace std;

char* Readfile(const std::string& file_path, uint64_t& len) {
  std::ifstream file(file_path.c_str(), std::ios::binary);
  if (!file.is_open()) {
      std::cerr << "Failed to open file: " << file_path << std::endl;
      return nullptr;
  }

  file.seekg(0, std::ios::end);
  len = file.tellg();
  if (len <= 0) {
      std::cerr << "File is empty or invalid size: " << file_path << std::endl;
      file.close();
      return nullptr;
  }

  file.seekg(0, std::ios::beg);

  const uint64_t chunk_size = 1024 * 1024; // 1MB per chunk
  uint64_t remaining = len;
  char* buf = new (std::nothrow) char[len];
  if (!buf) {
      std::cerr << "Memory allocation failed for file: " << file_path << std::endl;
      file.close();
      return nullptr;
  }

  char* current_pos = buf;
  while (remaining > 0) {
      uint64_t read_size = std::min(chunk_size, remaining);
      file.read(current_pos, read_size);
      if (file.gcount() != read_size) {
          std::cerr << "Failed to read complete file content: " << file_path << std::endl;
          file.close();
          delete[] buf;
          return nullptr;
      }
      current_pos += read_size;
      remaining -= read_size;
  }

  file.close();
  return buf;
}
int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s <path>\n", argv[0]);
  }

  uint64_t len = 0;
  char* buf = Readfile(argv[1], len);
  if (!buf) {
    printf("read fail\n");
	return 0;
  }

  file_block_digest::FileDigestInfo upload_info;
  // file_block_digest::GetFileDigestInfo(content.c_str(), content.size(), &upload_info);
  file_block_digest::GetFileDigestInfo(buf, len, &upload_info);

  printf("blocks: %zd\n", upload_info.parts.size());
  for (size_t i = 0; i < upload_info.parts.size(); ++i) {
    file_block_digest::BlockInfo & part = upload_info.parts[i];
    printf("part_num: %u end_offset: %lu cumulate_sha1: %s\n",
        part.part_num, part.end_offset, part.cumulate_sha1.c_str());
  }

  if (buf) {
    delete[] buf;
  }
  return 0;
}
