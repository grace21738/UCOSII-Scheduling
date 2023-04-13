## UCOS-II Rate Monotonic Scheduling

### Rate Monotonic 原理

永遠調度 priority 最高的 task，若有較高的 priority task 被釋放出來則會 preempt priority 較低的 task。

### 執行環境

+ Virtual Machine Windows 7
+ NiosII + QuartusII 

### 實作方法

1. 在 uCOSii.h structure OS_TCB 中新增 compute time ( comp ) 以及 period ( period )。
2. 緊接著在 OS_CORE.c 中新增四個部分:
   + 在 OSTimeTick() 中新增將 current TCB 的 compute time 減一的指令。
   + 在 OSIntExit() 中將 preempt 的訊息存入 buffer 中。
   + 在 OS_Sched() 中將 complete 的訊息存入 buffer 中。
   + 創建新的函式判斷是否有 task 的 deadline 大於 OSTime，並在 OSIntExit() 和 OS_Sched() 中呼叫此函式。
3. 在主程式的 Task() 中更新 deadline 以及 compute time。
4. 最後在主程式的 Task() 判斷存放事件狀態的 buffer 是否滿了或是否有 task timeout ，若 buffer 滿了或當出現 timeout 時則印出所有資訊。 

### 執行例子以及效果

1.  $T_1( 1,3 ), \ \ \ T_2(3,6)$

 <img src=".\images\IMG_8955.jpg" alt="IMG_8955" style="zoom:50%;" />

2.  $T_1( 1,3 ), \ \ \ T_2(3,6) , \ \ \ T_2(6,9)$

<img src=".\images\IMG_8947.jpg" alt="IMG_8947" style="zoom:33%;" />