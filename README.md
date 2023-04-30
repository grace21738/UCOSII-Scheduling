# UCOS-II Scheduling

## Scheduling 原理

+ Rate Monotonic : 永遠調度 priority 最高的 task，若有較高的 priority task 被釋放出來則會 preempt priority 較低的 task。
+ EDF : Early Deadline First，優先執行最先逾期的 task。

### 執行環境

+ Virtual Machine Windows 7
+ NiosII + QuartusII 

### 實作方法

+ Rate Monotic
  1. 在 uCOSii.h structure OS_TCB 中新增 compute time ( comp ) 以及 period ( period )。
  2. 緊接著在 OS_CORE.c 中新增四個部分:
     + 在 OSTimeTick() 中新增將 current TCB 的 compute time 減一的指令。
     + 在 OSIntExit() 中將 preempt 的訊息存入 buffer 中。
     + 在 OS_Sched() 中將 complete 的訊息存入 buffer 中。
     + 創建新的函式 check_timeout() 判斷是否有 task 的 deadline 大於 OSTime，並在 OSIntExit() 和 OS_Sched() 中呼叫此函式。


+ EDF
  1. 在 uCOSii.h structure OS_TCB 中新增 compute time ( comp ) 以及 period ( period ) 和 deadline 。
  2. 緊接著在 OS_CORE.c 中新增四個部分:
     + 在 OSTimeTick() 中新增將 current TCB 的 compute time 減一的指令。
     + 在 OSIntExit() 中將 preempt 的訊息存入 buffer 中。
     + 在 OS_Sched() 中將 complete 的訊息存入 buffer 中。
     + 更改函式 NewSched() 判斷是否有新的 task 的 deadline 比目前執行的 task 較短，若有則更改 OSHighPriority ，並在 OSIntExit() 和 OS_Sched() 中呼叫此函式。( 若目前正在執行的 task deadline 和其他 Ready task 相同，則會先執行完目前的  task 直到 Complete 或有更小的 deadline task 進來為止 )

3. 在 OSStart() 執行之前初始化每個 task 的 compute time 和 deadline。
4. 在主程式的 Task() 中更新 deadline 以及 compute time。
5. 最後在主程式的 Task() 判斷存放事件狀態的 buffer 是否滿了或是否有 task timeout ，若 buffer 滿了或當出現 timeout 時則印出所有資訊。 

### 執行例子以及效果

+ Rate Monotic
  1. $T_1( 1,3 ), \ \ \ T_2(3,6)$

 <img src=".\images\IMG_8955.jpg" alt="IMG_8955" style="zoom: 33%;" />

​	2. $T_1( 1,3 ), \ \ \ T_2(3,6) , \ \ \ T_2(6,9)$

<img src=".\images\IMG_8947.jpg" alt="IMG_8947" style="zoom:33%;" />

+ EDF 

  1. $T_1( 1,3 ), \ \ \ T_2(3,5)$

     <img src=".\images\EDF_2.jpg" alt="EDF_1" style="zoom:33%;" />

  2. $T_1( 1,4 ), \ \ \ T_2(2,5)\ \ \ T_2(2,10)$

     <img src=".\images\EDF_2.jpg" alt="EDF_1" style="zoom:33%;" />