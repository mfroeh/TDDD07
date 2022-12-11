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
void task_communicate(void) {
    // Check if task is enabled
    if (g_task_communicate.enabled == s_TRUE) {
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

        // Send queue that will be filled with victims
        queue_t *victims = queue_init();
        queue_t *maps = queue_init();
        queue_t *positions = queue_init();
        queue_t *streams = queue_init();

        // How many items of the specific type were in the send_list
        static int found[5];

        // Traverse send list
        while (g_list_send->count != 0) {
            seq++;

            queue_t *queue;
            // Allocate memory for data structure
            switch (g_list_send->first->data_type) {
                // Robot pose
                case s_DATA_STRUCT_TYPE_ROBOT :
                    data = (void *) malloc(sizeof(robot_t));
                    queue = positions;
                    printf("Got robot position\n");
                    queue_empty(positions);
                    break;
                    // Victim information
                case s_DATA_STRUCT_TYPE_VICTIM :
                    data = (void *) malloc(sizeof(victim_t));
                    queue = victims;
                    printf("Got victim position\n");
                    break;
                    // Pheromone map
                case s_DATA_STRUCT_TYPE_PHEROMONE :
                    data = (void *) malloc(sizeof(pheromone_map_sector_t));
                    queue = maps;
                    printf("Got pheromone map\n");
                    // Keep only last 8 messages
                    if (found[s_DATA_STRUCT_TYPE_PHEROMONE] % 8 == 0) {
                        printf("Clearing pheromones\n");
                        queue_empty(maps);
                    }
                    break;
                    // Command (for future use)
                case s_DATA_STRUCT_TYPE_CMD :
                    data = (void *) malloc(sizeof(command_t));
                    break;
                case s_DATA_STRUCT_TYPE_STREAM :
                    data = (void *) malloc(sizeof(stream_t));
                    queue = streams;
                    printf("Got stream data\n");
                    break;
                    // Other
                default :
                    // Do nothing
                    continue;
                    break;
            }

            // Increment found count
            found[g_list_send->first->data_type]++;

            // Get data from the list and push to chosen queue
            doublylinkedlist_remove(g_list_send, g_list_send->first, data, &data_type);
            queue_enqueue(queue, data, data_type);

            // Free memory
            free(data);
        }

        printf("Sending start\n");

        // How many bytes we can still send
        int bytes_left = 153600 / 8 / 8; // bits per second / timeslot / byte count

        static int sent[5];

        // Send victim messages
        for (int i = 0; i < found[s_DATA_STRUCT_TYPE_VICTIM]; ++i) {
            data = (void *) malloc(sizeof(victim_t));
            queue_dequeue(victims, data, &data_type);
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

            if ((bytes_left -= udp_packet_len) >= 0) {
                udp_broadcast(g_udps, udp_packet, udp_packet_len);
                sent[data_type]++;
            }
            // Free memory
            free(data);
        }

        // Send position
        for (int i = 0; i < 1; ++i) {
            data = (void *) malloc(sizeof(robot_t));
            queue_dequeue(positions, data, &data_type);

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
            if ((bytes_left -= udp_packet_len) >= 0) {
                udp_broadcast(g_udps, udp_packet, udp_packet_len);
                sent[data_type]++;
            }
            // Free memory
            free(data);
        }

        // Send pheromones
        for (int i = 0; i < 8; ++i) {
            data = (void *) malloc(sizeof(pheromone_map_sector_t));
            queue_dequeue(maps, data, &data_type);

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
            if ((bytes_left -= udp_packet_len) >= 0) {
                udp_broadcast(g_udps, udp_packet, udp_packet_len);
                sent[data_type]++;
            }
            // Free memory
            free(data);
        }

        // Send stream data if possible
        for (int i = 0; i < found[s_DATA_STRUCT_TYPE_STREAM]; ++i) {
            data = (void *) malloc(sizeof(stream_t));
            queue_dequeue(streams, data, &data_type);

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
            if ((bytes_left -= udp_packet_len) >= 0) {
                udp_broadcast(g_udps, udp_packet, udp_packet_len);
                sent[data_type]++;
            }
            // Free memory
            free(data);
        }

        printf("Sending done\n");

        /* --- Receive Data --- */
        // Receive packets, decode and forward to proper process

        // Receive UDP packet
        while (udp_receive(g_udps, udp_packet, &udp_packet_len) == s_OK) {
            // Decode packet
            //printf("%s\n",udp_packet);
            if (protocol_decode(&packet, udp_packet, udp_packet_len, g_config.robot_id, g_config.robot_team) == s_OK) {
                // Now decoding depends on the type of the packet
                switch (packet.type) {
                    // ACK
                    case s_PROTOCOL_TYPE_ACK :
                        // Do nothing
                        break;

                        //Massi: go_ahead packet
                    case s_PROTOCOL_TYPE_GO_AHEAD : {
                        // Declare go ahead command
                        command_t go_ahead;
                        go_ahead.cmd = s_CMD_GO_AHEAD;
                        // Redirect to mission by adding it to the queue
                        queue_enqueue(g_queue_mission, &go_ahead, s_DATA_STRUCT_TYPE_CMD);

                        // Debuging stuff
                        debug_printf("GO_AHEAD RECEIVED for robot %d team %d\n", packet.recv_id, packet.send_team);
                        // Calculate time from packet (ms and s)
                        int send_time_s = floor(packet.send_time / 1000);
                        int send_time_ms = packet.send_time % 1000;
                        int now = floor(((long long) timelib_unix_timestamp() % 60000) / 1000);
                        debug_printf("GO_AHEAD_TIME: %d (%d)\n", send_time_s, now);

                        break;
                    }
                        // Data
                    case s_PROTOCOL_TYPE_DATA :
                        // Continue depending on the data type
                        switch (packet.data_type) {
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
                if (packet.type == s_PROTOCOL_TYPE_DATA)
                    free(packet.data);
            }
        }

        // Increase msg sequance id
        g_message_sequence_id++;
    }
}
