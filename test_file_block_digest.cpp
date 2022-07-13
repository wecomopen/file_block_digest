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

string Readfile(const string& file_path) {
  ifstream file(file_path.c_str());
  if (!file.is_open()) {
    return "";
  }
  int len;
  file.seekg(0, ios::end);
  len = file.tellg();
  file.seekg(0, ios::beg);
  if (len == 0) {
    file.close();
    return "";
  }
  char *buf = new char[len];
  file.read(buf, len);
  file.close();
  string content(buf, len);
  delete [] buf;
  return content;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s <path>\n", argv[0]);
  }

  string content = Readfile(argv[1]);

  file_block_digest::FileDigestInfo upload_info;
  file_block_digest::GetFileDigestInfo(content.c_str(), content.size(), &upload_info);

  printf("blocks: %zd\n", upload_info.parts.size());
  for (size_t i = 0; i < upload_info.parts.size(); ++i) {
    file_block_digest::BlockInfo & part = upload_info.parts[i];
    printf("part_num: %u end_offset: %lu cumulate_sha1: %s\n",
        part.part_num, part.end_offset, part.cumulate_sha1.c_str());
  }

  return 0;
}
