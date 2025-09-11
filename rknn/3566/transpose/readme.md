# play with transpose operator



## TestModel1 - problematic permute

put it next to a Conv (or another NPU op) so the compiler can sink/absorb it.


```
class TestModel1(nn.Module):
    def __init__(self, ):
        super(TestModel1, self).__init__()

    def forward(self, x):        
        # Transpose [0,1,2,3] -> [0,1,3,2]
        return x.permute(0, 1, 3, 2)
```

```
D RKNN: [12:52:02.805] ID   OpType             DataType Target InputShape                               OutputShape            Cycles(DDR/NPU/Total)    RW(KB)       FullName        
D RKNN: [12:52:02.805] --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
D RKNN: [12:52:02.805] 0    InputOperator      FLOAT16  CPU    \                                        (1,2,3,4)              0/0/0                    0            InputOperator:onnx::Transpose_0
D RKNN: [12:52:02.805] 1    Transpose          FLOAT16  CPU    (1,2,3,4)                                (1,2,4,3)              0/0/0                    0            Transpose:/Transpose
D RKNN: [12:52:02.805] 2    OutputOperator     FLOAT16  CPU    (1,2,4,3)                                \                      0/0/0                    0            OutputOperator:1


```

On Rockchip, a standalone Transpose is usually treated as a layout/data-movement op and the toolkit will run it on CPU unless it can fuse it away into a surrounding NPU op (Conv/MatMul/etc.). The NPU doesn’t generally execute a generic “transpose kernel” as a separate op. Rockchip’s own logs and issues also show Transpose falling back to CPU, with extra size limits when it is handled internally (e.g., H×W must be below a threshold); otherwise it’s CPU.




## TestModel2 - Fold through a Conv


x.permute(0,2,3,1) → NCHW → NHWC: (1,2,3,4) → (1,3,4,2)

x.permute(0,3,1,2) → NHWC → NCHW: (1,3,4,2) → (1,2,3,4)


```
class TP_Fold_Conv(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv = nn.Conv2d(2, 4, kernel_size=3, padding=1, bias=False)

    def forward(self, x):                   # x: [N,2,H,W]
        x = x.permute(0, 2, 3, 1)          # NCHW -> NHWC
        x = x.permute(0, 3, 1, 2)          # NHWC -> NCHW (so Conv2d can consume)
        return self.conv(x)   

```

```

D RKNN: [13:02:52.069] 1    Conv               FLOAT16  NPU    (1,2,3,4),(4,2,3,3)                      (1,4,3,4)              88/144/144               0            Conv:/conv/Conv 
D RKNN: [13:02:52.069] 2    OutputOperator     FLOAT16  NPU    (1,4,3,4)                                \                      0/0/0                    0            OutputOperator:2




this is npu shape is :      In-channels = 2, out-channels = 4, kernel=3, padding=1 

```