//FreeRTOS queue example

//use only core 1 
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

static const uint8_t msg_queue_len = 5;

static QueueHandle_t msg_queue;

//task: wait for item on queue and print it 
void printMessages(void *parameters){

  int item;

  while(1){

    //check if there's a message in the queue
    if(xQueueReceive(msg_queue, (void*)&item, 0) == pdTRUE){
      Serial.println(item);
    }

    //wait before trying again
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void setup() {

  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("FreeRTOS queue example");

  //create queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  // Start print task
  xTaskCreatePinnedToCore(printMessages,
                          "Print Messages",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

}

void loop() {
  
static int num  = 0;

  // Try to add item to queue for 10 ticks, fail if queue is full
  if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE) {
    Serial.println("Queue full");
  }
  num++;

  // Wait before trying again
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
