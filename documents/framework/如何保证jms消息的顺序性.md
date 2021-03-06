http://sulong.me/2009/06/03/how_to_keep_jms_message_sequence

# 如何保证jms消息的顺序性

{[0 Comments](http://sulong.me/2009/06/03/how_to_keep_jms_message_sequence#respond)}

Posted by [sulong](http://sulong.me/author/sulong) on 2009/06/03

 in [Java EE](http://sulong.me/category/javaee), [理论](http://sulong.me/category/%e7%90%86%e8%ae%ba)

Tagged: [java](http://sulong.me/tag/java), [java EE](http://sulong.me/tag/java-ee), [jms](http://sulong.me/tag/jms)

JMS提供的queue和topic两种工作方式，其中queue能保证消息在传输中的顺序性，这是队列先入先出(first in first out)的特性。JMS本身不能保证多个队列里的消息的顺序性，比如 先放入queue1的消息m1，并不一定总是比后放入queue2的消息m2，先到达同样的目的地。

并发地发送和接收消息不能保证消息按正确顺序进入队列和被消费。比如，有两个线程分别发送创建订单和支付订单消息，由于线程运行的不确定性，即使我们按顺序先开始发送创建订单的线程，后开始支付订单的线程，如果两个线程开始时间间隔不够长，还是有可能后开始的线程先发送掉消息。同样的情况发生在并发的接收处理消息时。

那么如何保证消息的顺序性呢？

一种方案是，乱序接收，顺序处理。也就是说，消息在发送，传输和接收过程中可能是乱序的，但消费者在接收到消息之后，并不立即处理，而是先将消息排序，然后在处理。JMS消息头部的 JMSCorrelationID可以帮助我们完成这个工作。JMSCorrelationID存放了另一个消息的id。消息的发送者，如果要保证消息的顺序性，要将后发送的消息的JMSCorrelationID设置成前一个消息的id。消费者接收消息后，如果发现其头部有JMSCorrelationID，则查看该消息是否已被处理过，如果没有，则等待该消息，至到该消息被处理后，才处理这个消息。这一工作需要发送者和接收者都记住已经发送和接收过的消息，以便于给后来的消息参考。

另一种方案是，顺序发送，顺序传输，顺序接收，顺序处理。其中传输可以由queue来保证，但是发送接收和处理则需要应用程序来控制。简单的说，直到前一个消息发送成功，才能发送后一个消息，同样的，直到前一个消息被接受和处理结束，才能接收和处理后一个消息。这样的做法无疑会降低并发带来的好处。

以上所说的方案，是用来严格控制消息的顺序性的，然而，如果消息的发送的间隔时间足够长，不需要做过多控制，也可以控制消息的顺序性。假设，一个消息正常情况下，由发送，传输，接收到处理成功，最大时间耗费是2秒，那么只要我们保证消息发送的时间隔达不低于2秒，那么这两个消息就可以被正确的处理。这一条件咋听起来很苛刻，但事实上大部分的消息都满足了。比如，同一个订单的创建，和请求支付，最快也要数秒的时间，而退款的开始和结束，可能要数天。在典型的web应用中，操作人员不可能那么快的点击系统，而系统也不可能那么快的响应。进一步的，如果两个需要顺序处理的业务事件可能在极端的时间里连续发生，我们只要在程序上控制，人为将间隔拉开，就可以保证顺序性。

在系统的各各环节控制消息的顺序，代价高昂，而带来的好处却只是针对那极少数极端情况；通过业务或程序的方式，保证消息的有效时间间隔，代价较小，也能有效保证顺序。

如果系统中有很多的间隔极端，又需要保证顺序的消息，那你就要考虑是否将这两个消息合并成一个，或则不该采用jms了。

