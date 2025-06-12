# 描述
上传大文件（最高支持20G）至企业微信的微盘，需要开发者在本地对大文件进行分块以及计算分块的`累积sha值`（签名）。

![](https://wework.qpic.cn/wwpic/96231_Zl6gOI-1TG6WokZ_1657629301/0)

分块的`累积sha值`计算过程如下：
- 将要上传的文件内容，按2M分块；
- 对每一个分块，依次sha1_update；
- 每次update，记录当前的state，转成16进制，作为**当前块**的`累积sha值`；
- 当为最后一块（可能小于2M），update完再sha1_final得到的sha1值（即整个文件的sha1），作为**最后一块**的`累积sha值`。

以上过程得到的sha值，保持顺序依次放到数组，作为`file_upload_init`接口的`block_sha`参数输入。

# 使用
sha1.* 可直接替换成其他sha1的实现，如openssl sha1。

linux下执行以下命令，生成 test_file_block_digest 二进制工具

aclocal

automake --add-missing

./configure  CXX=g++ CPPFLAGS=-std=c++11

make


或直接使用g++编译得到 test_file_block_digest 二进制工具

g++ -std=c++11 file_block_digest.h file_block_digest.cpp sha1.h sha1.cpp test_file_block_digest.cpp -o test_file_block_digest

Windows下执行以下命令可以将test_file_block_digest.cpp编译为exe可执行文件

g++ -static test_file_block_digest.cpp file_block_digest.cpp sha1.cpp -o test_file_block_digest.exe
