/**
 * @file	task_communicate.c
 * @author  Eriks Zaharans
 * @date    31 Oct 2013
 *
 * @section DESCRIPTION
 *
 * Communicate Task.
 */

/* -- Includes -- */
/* system libraries */

/* project libraries */
#include "task.h"



/* int queue_send(queue_t* queue) { */
/*   void* data = NULL; */
/*   int data_type; */
/*   queue_dequeue(queue, data, &data_type); */
/*   if (data == NULL) return 0; */

  /* // Encode data into UDP packet */
  /* protocol_encode(udp_packet, */
  /*     &udp_packet_len, */
  /*     s_PROTOCOL_ADDR_BROADCAST, */
  /*     g_config.robot_id, */
  /*     g_config.robot_team, */
  /*     s_PROTOCOL_TYPE_DATA, */
  /*     seq, */
  /*     g_message_sequence_id, */
  /*     last_id, */
  /*     data_type, */
  /*     data); */

  /* // Broadcast packet */
  /* udp_broadcast(g_udps, udp_packet, udp_packet_len); */

/*   free(data); */

/*   return 1; */
/* } */

/**
 * Communication (receive and send data)
 */
void task_communicate(void)
{
  // Check if task is enabled
  if(g_task_communicate.enabled == s_TRUE)
  {
    // Loacal variables
    void *data; // Void pointer for data
    int data_type; // Data type

    // UDP Packet
    char udp_packet[g_config.udp_packet_size];
    int udp_packet_len;

    // Protocol
    protocol_t packet;

    //Start the new sequence
    int seq = 0; // Massi thing
    //In principle I want to send all the data in the buffer
    int last_id = g_list_send->count; // Massi thing


    // --------------------------------------------------
    //	LAB 2 starts here
    // --------------------------------------------------

    // NOTE: Should be able to send 153600 / 8 / 8 = 2400 bytes
    // Assume that robot clock is the same as mission control clock
    // Has to start EXACTLY on the slot

    printf("Send start\n");


    // Send queue that will be filled with victims
    queue_t* victims = queue_init();
    queue_t* maps = queue_init();
    queue_t* positions = queue_init();
    queue_t* streams = queue_init();

    // How many items of the specific type were in the send_list
    static int found[5];

    /* --- Send Data --- */
    while(g_list_send->count != 0)
    {
      seq++;
      found[g_list_send->last->data_type]++;

      queue_t* queue;
      // Allocate memory for data structure
      switch(g_list_send->last->data_type)
      {
        // Robot pose
        case s_DATA_STRUCT_TYPE_ROBOT :
          data = (void *)malloc(sizeof(robot_t));
          queue = positions;
          printf("Got robot position\n");
          break;
          // Victim information
        case s_DATA_STRUCT_TYPE_VICTIM :
          data = (void *)malloc(sizeof(victim_t));
          queue = victims;
          printf("Got victim position\n");
          break;
          // Pheromone map
        case s_DATA_STRUCT_TYPE_PHEROMONE :
          data = (void *)malloc(sizeof(pheromone_map_sector_t));
          queue = maps;
          printf("Got pheromone map\n");
          break;
          // Command (for future use)
        case s_DATA_STRUCT_TYPE_CMD :
          data = (void *)malloc(sizeof(command_t));
          break;
        case s_DATA_STRUCT_TYPE_STREAM :
          data = (void *)malloc(sizeof(stream_t));
          queue = streams;
          printf("Got stream data\n");
          break;
          // Other
        default :
          // Do nothing
          continue;
          break;
      }

      // Get data from the list
      doublylinkedlist_remove(g_list_send, g_list_send->last, data, &data_type);
      queue_enqueue(queue, data, data_type);

    }

    printf("\n1");


    data = (void*)malloc(sizeof(robot_t));
    queue_dequeue(positions, data, &data_type);
    if (data != NULL) {
      // Encode data into UDP packet
      protocol_encode(udp_packet,
          &udp_packet_len,
          s_PROTOCOL_ADDR_BROADCAST,
          g_config.robot_id,
          g_config.robot_team,
          s_PROTOCOL_TYPE_DATA,
          seq,
          g_message_sequence_id,
          last_id,
          data_type,
          data);

      // Broadcast packet
      udp_broadcast(g_udps, udp_packet, udp_packet_len);
    }

    // free(data);


    // Send all the victim information 

    // Idea
    /* if (g_list_send->count != 0) { */

    /*   printf("1"); */

    /*   /1* #define s_DATA_STRUCT_TYPE_ROBOT		0 *1/ */
    /*   /1* #define s_DATA_STRUCT_TYPE_VICTIM		1 *1/ */
    /*   /1* #define s_DATA_STRUCT_TYPE_PHEROMONE	2 *1/ */
    /*   /1* #define s_DATA_STRUCT_TYPE_CMD			3 *1/ */
    /*   /1* #define s_DATA_STRUCT_TYPE_STREAM		4 *1/ */

    /*   // Traverse send list in reverse */
    /*   doublylinkedlist_node_t* node = g_list_send->last; */

    /*   printf("2"); */
    /*   while(node != NULL) */
    /*   { */
    /*     seq++; */
    /*     printf("3"); */
    /*     switch (node->data_type) { */
    /*       case s_DATA_STRUCT_TYPE_VICTIM: */
    /*         queue_enqueue(queue, node->data, node->data_type); */
    /*         break; */
    /*       case s_DATA_STRUCT_TYPE_ROBOT: */
    /*         if (map == NULL) pos = node->data; */
    /*         break; */
    /*       case s_DATA_STRUCT_TYPE_PHEROMONE: */
    /*         if (map == NULL) map = node->data; */
    /*         break; */
    /*       default: */
    /*         printf("Got unhandled data type %d", node->data_type); */
    /*         break; */
    /*     } */

    /*     kjkjk */
    /*       printf("4"); */
    /*     printf("5"); */
    /*     node = node->prev; */
    /*   } */

    /*   // Empty the send list for reuse */
    /*   doublylinkedlist_empty(g_list_send); */
    /*   printf("6"); */

    /*   // Append position and map to queue */
    /*   queue_enqueue(queue, pos, s_DATA_STRUCT_TYPE_ROBOT); */
    /*   queue_enqueue(queue, map, s_DATA_STRUCT_TYPE_PHEROMONE); */
    /*   printf("6"); */

    /*   // TODO */
    /*   /1* Data Structure types *1/ */


    /*   // TODO: Fill up the rest with stream data */

    /* } */
    /* /1* int bytes_sent = 0; *1/ */

    /* /1* // How many items we sent per data type *1/ */
    /* /1* static int sent[5]; *1/ */

    /* /1* // Send items in send queue *1/ */
    /* /1* queue_dequeue(queue, data, &data_type); *1/ */
    /* /1* while (data != NULL) { *1/ */
    /* /1*   // Encode data into UDP packet *1/ */
    /* /1*   protocol_encode(udp_packet, *1/ */
    /* /1*       &udp_packet_len, *1/ */
    /* /1*       s_PROTOCOL_ADDR_BROADCAST, *1/ */
    /* /1*       g_config.robot_id, *1/ */
    /* /1*       g_config.robot_team, *1/ */
    /* /1*       s_PROTOCOL_TYPE_DATA, *1/ */
    /* /1*       seq, *1/ */
    /* /1*       g_message_sequence_id, *1/ */
    /* /1*       last_id, *1/ */
    /* /1*       data_type, *1/ */
    /* /1*       data); *1/ */

    /* /1*   // Broadcast packet *1/ */
    /* /1*   udp_broadcast(g_udps, udp_packet, udp_packet_len); *1/ */

    /* /1*   // TODO: Free the sent item *1/ */
    /* /1*   /2* free(data); *2/ *1/ */
    /* /1*   data = null; *1/ */

    /* /1*   bytes_sent += udp_packet_len; *1/ */
    /* /1*   sent[data_type]++; *1/ */

    /* /1*   printf("Sent another packet of type %d! Now sent %d bytes in this task execution.", data_type, bytes_sent); *1/ */

    /* /1*   // Get next item from queue *1/ */
    /* /1*   queue_dequeue(queue, data, &data_type); *1/ */
    /* /1* } *1/ */

    /* /1* // Free the queue *1/ */
    /* /1* queue_destroy(queue); *1/ */
    /* /1* } *1/ */

    /* --- Receive Data --- */
    // Receive packets, decode and forward to proper process

    // Receive UDP packet
    while(udp_receive(g_udps, udp_packet, &udp_packet_len) == s_OK)
    {
      // Decode packet
      //printf("%s\n",udp_packet);
      if(protocol_decode(&packet, udp_packet, udp_packet_len, g_config.robot_id, g_config.robot_team) == s_OK)
      {
        // Now decoding depends on the type of the packet
        switch(packet.type)
        {
          // ACK
          case s_PROTOCOL_TYPE_ACK :
            // Do nothing
            break;

            //Massi: go_ahead packet
          case s_PROTOCOL_TYPE_GO_AHEAD :
            {
              // Declare go ahead command 
              command_t go_ahead;
              go_ahead.cmd = s_CMD_GO_AHEAD;
              // Redirect to mission by adding it to the queue
              queue_enqueue(g_queue_mission, &go_ahead, s_DATA_STRUCT_TYPE_CMD);

              // Debuging stuff
              debug_printf("GO_AHEAD RECEIVED for robot %d team %d\n",packet.recv_id,packet.send_team);
              // Calculate time from packet (ms and s)
              int send_time_s = floor(packet.send_time / 1000);
              int send_time_ms = packet.send_time % 1000;
              int now = floor(((long long)timelib_unix_timestamp() % 60000) / 1000);
              debug_printf("GO_AHEAD_TIME: %d (%d)\n",send_time_s,now);

              break;
            }
            // Data
          case s_PROTOCOL_TYPE_DATA :
            // Continue depending on the data type
            switch(packet.data_type)
            {
              // Robot pose
              case s_DATA_STRUCT_TYPE_ROBOT :
                debug_printf("received robot\n");
                // Do nothing
                break;
                // Victim information
              case s_DATA_STRUCT_TYPE_VICTIM :
                debug_printf("received victim\n");
                // Redirect to mission by adding it to the queue
                queue_enqueue(g_queue_mission, packet.data, s_DATA_STRUCT_TYPE_VICTIM);
                break;
                // Pheromone map
              case s_DATA_STRUCT_TYPE_PHEROMONE :
                debug_printf("received pheromone\n");
                // Redirect to navigate by adding it to the queue
                queue_enqueue(g_queue_navigate, packet.data, s_DATA_STRUCT_TYPE_PHEROMONE);

                break;
                // Command
              case s_DATA_STRUCT_TYPE_CMD :
                debug_printf("received CMD\n");
                // Redirect to mission by adding it to the queue
                queue_enqueue(g_queue_mission, packet.data, s_DATA_STRUCT_TYPE_CMD);
                break;
              case s_DATA_STRUCT_TYPE_STREAM :
                debug_printf("received data stream item\n");
                break;
                // Other
              default :
                // Do nothing
                break;
            }
            // Other ?
          default:
            // Do nothing
            break;
        }

        // Free memory (only if data packet was received!)
        if(packet.type == s_PROTOCOL_TYPE_DATA)
          free(packet.data);
      }
    }

    // Increase msg sequance id
    g_message_sequence_id++;
}
}
