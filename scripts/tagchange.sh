#!/bin/bash

# 检查是否传入了两个参数
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <tag_name> <message>"
    exit 1
fi

# 获取参数
tag_name=$1
message=$2

# 删除远程和本地旧标签
git push origin --delete "$tag_name"
git tag -d "$tag_name"

# 创建新的标签并推送
git tag "$tag_name" -m "$message"
git push origin --tags
