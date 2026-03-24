from phe import paillier
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import random
import os

##################### 第一部分：初始化密钥
# 客户端生成 Paillier 密钥对
public_key, private_key = paillier.generate_paillier_keypair()

# 客户端生成对称密钥 k（AES-128）
symm_key = os.urandom(16)

##################### 第二部分：服务器端数据准备
# 服务器持有的明文消息列表
message_list = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
length = len(message_list)

# 用对称密钥加密每条消息
ciphertext_list = []
for message in message_list:
    message_bytes = str(message).encode('utf-8')
    cipher = AES.new(symm_key, AES.MODE_ECB)
    padded_message = pad(message_bytes, AES.block_size)
    ciphertext = cipher.encrypt(padded_message)
    ciphertext_list.append(ciphertext)

print("服务器已加密并保存所有消息")

##################### 第三部分：客户端构造加密选择向量
# 客户端选择要查询的位置
pos = random.randint(0, length - 1)
print(f"\n客户端要查询的位置：{pos}")

# 构造并加密选择向量
select_list = []
enc_list = []
for i in range(length):
    select_list.append(i == pos)
    enc_list.append(public_key.encrypt(int(select_list[i])))

print("客户端发送加密选择向量给服务器")

##################### 第四部分：服务器在密文域执行计算
# 将密文列表转换为整数（用于加权求和）
ciphertext_int_list = [int.from_bytes(ct, byteorder='big') for ct in ciphertext_list]

# 在密文域执行加权求和
result = public_key.encrypt(0)
for i in range(length):
    result += ciphertext_int_list[i] * enc_list[i]

print("服务器完成加权求和，返回密文结果")

##################### 第五部分：客户端解密
# 解密得到目标密文的整数表示
decrypted_int = private_key.decrypt(result)

# 将整数转换回字节形式（需要记录密文长度）
ciphertext_len = len(ciphertext_list[pos])
target_ciphertext = int(decrypted_int).to_bytes(ciphertext_len, byteorder='big')

print(f"客户端获得目标密文：{target_ciphertext.hex()}")

# 用对称密钥解密
decipher = AES.new(symm_key, AES.MODE_ECB)
decrypted_bytes = decipher.decrypt(target_ciphertext)
decrypted_message = int(unpad(decrypted_bytes, AES.block_size).decode('utf-8'))

print(f"客户端解密得到原始消息：{decrypted_message}")
print(f"期望值：{message_list[pos]}")
print(f"验证结果：{'成功' if decrypted_message == message_list[pos] else '失败'}")
