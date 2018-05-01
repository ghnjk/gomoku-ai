gomoku-ai
===============

  该项目主要是学习神经网络算法的demo

安装
-----

```
pip install -r required.txt
```

执行
-----

```
单线程训练
python train.py

并发训练
需要结合机器中具体cpu情况, 修改train_multi_process.py中
        workerCount = 12   # self-play 并发数
        trainBatchPerIter = 10 # 每轮并发self-play 随机训练次数， 一般和workerCount差不多
python train_multi_process.py
```