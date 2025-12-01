/**
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * @brief template main.cpp file for Assignment 3 Part 1 of SYSC4001
 * 
 */

/**
 * @file interrupts_student1_student2_RR.cpp
 * @brief Round Robin (RR) scheduler implementation for Assignment 3 Part 1
 */

#include "interrupts_student1_student2.hpp"


static std::vector<PCB>::iterator find_best_ready(std::vector<PCB> &ready_queue) {
    if (ready_queue.empty()) return ready_queue.end();

    auto best_it = ready_queue.begin();
    unsigned int best_prio = best_it->priority;

    for (auto it = std::next(ready_queue.begin()); it != ready_queue.end(); ++it) {
        if (it->priority < best_prio) {
            best_prio = it->priority;
            best_it = it;
        }
    }
    return best_it;
}

// Dispatch: move best READY process into RUNNING
static bool dispatch_best(
    PCB &running,
    std::vector<PCB> &job_list,
    std::vector<PCB> &ready_queue,
    unsigned int current_time,
    std::string &execution_status
) {
    auto best_it = find_best_ready(ready_queue);
    if (best_it == ready_queue.end()) return false;

    PCB next = *best_it;
    ready_queue.erase(best_it);

    running = next;

    if (running.start_time == -1) {
        running.start_time = static_cast<int>(current_time);
    }

    running.state = RUNNING;
    sync_queue(job_list, running);

    execution_status +=
        print_exec_status(current_time, running.PID, READY, RUNNING);

    return true;
}

std::tuple<std::string> run_simulation(std::vector<PCB> list_processes) {

    std::vector<PCB> ready_queue;
    std::vector<PCB> wait_queue;
    std::vector<PCB> job_list;

    unsigned int current_time = 0;
    PCB running;
    idle_CPU(running);

    std::string execution_status = print_exec_header();

    const unsigned int QUANTUM = 100;
    unsigned int time_slice = 0;

    while (!all_process_terminated(job_list) || job_list.empty()) {

        // 1) Populate the ready queue with processes as they arrive
        for (auto &process : list_processes) {
            if (process.arrival_time == current_time) {

                bool loaded = assign_memory(process);
                if (!loaded) {
                    continue;
                }

                process.state = READY;
                process.time_since_last_io = 0;
                process.io_remaining       = 0;

                ready_queue.push_back(process);
                job_list.push_back(process);

                execution_status +=
                    print_exec_status(current_time, process.PID, NEW, READY);
            }
        }

    
        for (auto it = wait_queue.begin(); it != wait_queue.end(); ) {

            it->io_remaining--;

            if (it->io_remaining == 0) {

                states old_state = WAITING;
                it->state = READY;
                it->time_since_last_io = 0;

                sync_queue(job_list, *it);
                ready_queue.push_back(*it);

                execution_status +=
                    print_exec_status(current_time,
                                      it->PID,
                                      old_state,
                                      READY);

                it = wait_queue.erase(it);
            } else {
                ++it;
            }
        }

    

        if (!ready_queue.empty()) {
            auto best_it = find_best_ready(ready_queue);
            if (running.PID == -1) {
                // CPU idle: just dispatch best
                dispatch_best(running, job_list, ready_queue, current_time, execution_status);
                time_slice = 0;
            } else {
                if (best_it->priority < running.priority) {

                    states old_state = RUNNING;
                    running.state = READY;
                    sync_queue(job_list, running);
                    ready_queue.push_back(running);

                    execution_status +=
                        print_exec_status(current_time,
                                          running.PID,
                                          old_state,
                                          READY);

                    running.PID = -1;
                    time_slice  = 0;

                    // Now dispatch  better one
                    dispatch_best(running, job_list, ready_queue, current_time, execution_status);
                }
            }
        } else if (running.PID == -1) {
        }

        
        if (running.PID != -1) {

            running.remaining_time--;
            if (running.io_freq > 0) {
                running.time_since_last_io++;
            }

            sync_queue(job_list, running);

            bool still_running = true;

            if (running.remaining_time == 0) {

                states old_state = RUNNING;
                terminate_process(running, job_list);

                execution_status +=
                    print_exec_status(current_time,
                                      running.PID,
                                      old_state,
                                      TERMINATED);

                running.PID = -1;
                time_slice  = 0;
                still_running = false;
            }
            else if (running.io_freq > 0 &&
                     running.time_since_last_io >= running.io_freq) {

                states old_state = RUNNING;
                running.state = WAITING;
                running.io_remaining = running.io_duration;
                running.time_since_last_io = 0;

                sync_queue(job_list, running);
                wait_queue.push_back(running);

                execution_status +=
                    print_exec_status(current_time,
                                      running.PID,
                                      old_state,
                                      WAITING);

                running.PID = -1;
                time_slice  = 0;
                still_running = false;
            }

            //  Quantum expiry
            if (still_running) {
                time_slice++;

                if (time_slice == QUANTUM) {

                    states old_state = RUNNING;
                    running.state = READY;
                    sync_queue(job_list, running);
                    ready_queue.push_back(running);

                    execution_status +=
                        print_exec_status(current_time,
                                          running.PID,
                                          old_state,
                                          READY);

                    running.PID = -1;
                    time_slice  = 0;
                }
            }
        }

        // Safety: break if nothing exists and nothing to arrive
        if (job_list.empty() && ready_queue.empty() && wait_queue.empty() && running.PID == -1) {
            bool any_future_arrivals = false;
            for (auto &p : list_processes) {
                if (p.arrival_time > current_time) {
                    any_future_arrivals = true;
                    break;
                }
            }
            if (!any_future_arrivals) {
                break;
            }
        }

        current_time++;
    }

    execution_status += print_exec_footer();
    return std::make_tuple(execution_status);
}

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cout << "ERROR!\nExpected 1 argument, received "
                  << argc - 1 << std::endl;
        std::cout << "To run the program, do: ./interrupts_EP_RR <your_input_file.txt>"
                  << std::endl;
        return -1;
    }

    auto file_name = argv[1];
    std::ifstream input_file(file_name);

    if (!input_file.is_open()) {
        std::cerr << "Error: Unable to open file: " << file_name << std::endl;
        return -1;
    }

    std::string line;
    std::vector<PCB> list_process;
    while (std::getline(input_file, line)) {
        if (line.empty()) continue;
        auto input_tokens = split_delim(line, ", ");
        auto new_process = add_process(input_tokens);
        list_process.push_back(new_process);
    }
    input_file.close();

    auto [exec] = run_simulation(list_process);

    write_output(exec, "execution.txt");

    return 0;
}