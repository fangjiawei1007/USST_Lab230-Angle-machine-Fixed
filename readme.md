此代码为护角机程序代码，由王亮师兄最后一次维护。由于时间久远，代码未生成xml文件。
师弟师妹如果需要查看源码需要重新生成工程文件导入IDE中进行代码查看。
烧录注意事项:师弟师妹们如果需要烧录，选择文件夹中的RELEASE即可;
烧录注意事项block0开始烧录STEPLDR.bin（4KB）。block5开始烧录hujiaoji.bin


本次修改bug(方佳伟)：死机现象:原因：设置值不当(Houdu > Pianxinju_2)导致分母为0 
具体为 -> i = Pianxinju_2 - Houdu/Pianxinju_2 -> i<0 导致后续kw[94] == 0,后续/kw[94] 导致死机 
做了双重保证 kw[94]
此次修改在i之前进行了判断以保证Pianxinju_2>Houdu
此次画面修改：厚度值范围: 3.0-12.0 定长偏心距范围: 12.0-15.0 打框偏心距范围:9.0-15.0

powered by 王亮
uploded and modified by 方佳伟 联系方式:fangjiawei2006@qq.com 电话:15000675593