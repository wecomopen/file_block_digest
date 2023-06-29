这里用 demo 文件 [sha_calc_demo.txt](https://github.com/wecomopen/file_block_digest/blob/main/demo/sha_calc_demo.txt) 为例，说明如何构造请求。
开发者可对比来自助排查问题。（注：此文件已上传过，可能会直接命中秒传，但可以对比相关的参数）

## demo文件基本信息
文件大小 `5999998`字节。

通过 test_file_block_digest 工具输出：<br>
part_num: 1 end_offset: 2097152 cumulate_sha1: 5186fee37e9f7e77a1f6bea8d4e32638d5186d44<br>
part_num: 2 end_offset: 4194304 cumulate_sha1: cf540965f3190bc0c6367147ab0ac64dc9eff6a0<br>
part_num: 3 end_offset: 5999998 cumulate_sha1: bda1e884e1cb23a5cbdea73d2e00e23baa72d2f5<br>

表明需要分`3`块来上传，对应的 block_sha为：<br>
5186fee37e9f7e77a1f6bea8d4e32638d5186d44<br>
cf540965f3190bc0c6367147ab0ac64dc9eff6a0<br>
bda1e884e1cb23a5cbdea73d2e00e23baa72d2f5<br>

## 构造请求
### 分块上传初始化
请求地址: https://qyapi.weixin.qq.com/cgi-bin/wedrive/file_upload_init?access_token=ACCESS_TOKEN

```json
{
    "spaceid": "SPACEID",
    "fatherid": "FATHERID",
    "selected_ticket": "SELECTED_TICKET",
    "file_name": "sha_calc_demo.txt",
    "size": 5999998,
    "block_sha": [
        "5186fee37e9f7e77a1f6bea8d4e32638d5186d44",
        "cf540965f3190bc0c6367147ab0ac64dc9eff6a0",
        "bda1e884e1cb23a5cbdea73d2e00e23baa72d2f5"
    ],
    "skip_push_card": false
}
```
特别注意，`size`和`block_sha`需要对应上。

### 分块上传每一块（多次调用）
请求地址: https://qyapi.weixin.qq.com/cgi-bin/wedrive/file_upload_part?access_token=ACCESS_TOKEN

```json
{
    "upload_key": "UPLOAD_KEY",
    "index": 1,
    "file_base64_content": "FILE_BASE64_CONTENT"
}
```

此demo文件，需要调此接口`3`次，以完成总共`3`个分块的上传。
|index| 原始文件块大小（字节） |`file_base64_content`大小（字节）|`file_base64_content`前`128`字节（非完整内容）|
| :-- | :--- |  :--- |  :--- |  
| 1 | 2097152 | 2796204 | MTIzNDU2Nzg5MA0KMTIzNDU2Nzg5MA0KMTIzNDU2Nzg5MA0KMTIzNDU2Nzg5MA0KMTIzNDU2Nzg5MA0KMTIzNDU2Nzg5MA0KMTIzNDU2Nzg5MA0KMTIzNDU2Nzg5MA0K |
| 2 | 2097152 | 2796204| OTANCjEyMzQ1Njc4OTANCjEyMzQ1Njc4OTANCjEyMzQ1Njc4OTANCjEyMzQ1Njc4OTANCjEyMzQ1Njc4OTANCjEyMzQ1Njc4OTANCjEyMzQ1Njc4OTANCjEyMzQ1Njc4 |
| 3 | 1805694| 2407592 | NTY3ODkwDQoxMjM0NTY3ODkwDQoxMjM0NTY3ODkwDQoxMjM0NTY3ODkwDQoxMjM0NTY3ODkwDQoxMjM0NTY3ODkwDQoxMjM0NTY3ODkwDQoxMjM0NTY3ODkwDQoxMjM0 |

对于此demo文件：
1. 前面`2`块，原始大小必为`2M`，对应的base64大小固定为`2796204`字节
2. 每一块的`file_base64_content`的前`128`字节（注意不是完整的`file_base64_content`内容!!），方便开发者对比

特别注意，`file_base64_content`参数一定要完整，可对比参数长度。


### 分块上传完成
请求地址: https://qyapi.weixin.qq.com/cgi-bin/wedrive/file_upload_finish?access_token=ACCESS_TOKEN

```json
{
    "upload_key": "UPLOAD_KEY"
}
```
前面如果没有错误，继续调用此接口完成上传即可。
