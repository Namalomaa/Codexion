*This activity has been created as part of the 42 curriculum by namaloma.*

# Codexion

## Description

Codexion is a multithreading and synchronization project written in C using the POSIX Threads (`pthread`) library.

The project simulates a group of coders sitting around a circular table and sharing a limited number of dongles. Each coder is represented by its own thread and repeatedly follows the same routine:

1. Acquire the two required dongles.
2. Compile.
3. Release the dongles.
4. Debug.
5. Refactor.

A coder must begin compiling again before reaching its burnout deadline.

The goal of the project is to coordinate concurrent threads safely while respecting shared-resource constraints and a configurable scheduling policy.

Codexion supports two schedulers:

- `fifo` — requests are prioritized by arrival order.
- `edf` — requests are prioritized by the earliest burnout deadline. If two requests have the same deadline, their arrival sequence is used as a deterministic tie-breaker.

Pending requests are stored in a priority heap according to the selected scheduling policy.

## Instructions

### Compilation

Compile the project with:

```bash
make
```

The project is compiled using:

```text
-Wall -Wextra -Werror -pthread
```

The resulting executable is:

```bash
./codexion
```

### Execution

Run the simulation using:

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

The arguments are:

- `number_of_coders` — number of coder threads and dongles in the simulation.
- `time_to_burnout` — maximum time in milliseconds between compile starts before a coder burns out.
- `time_to_compile` — time spent compiling, in milliseconds.
- `time_to_debug` — time spent debugging, in milliseconds.
- `time_to_refactor` — time spent refactoring, in milliseconds.
- `number_of_compiles_required` — minimum number of successful compiles required for each coder.
- `dongle_cooldown` — time in milliseconds before a released dongle may be granted again.
- `scheduler` — scheduling policy. Must be either `fifo` or `edf`.

Example:

```bash
./codexion 5 900 200 200 100 5 0 edf
```

The simulation terminates when every coder has completed the required number of compiles, or immediately when a coder burns out.

## Blocking cases handled

The project handles several concurrency problems that can occur when multiple threads compete for shared resources.

### Deadlock prevention

A coder requires two dongles before it can begin compiling.

Instead of allowing a coder to acquire one dongle and then block indefinitely while waiting for the second, dongles are reserved together while the scheduler mutex is held.
A request is granted only when both required dongles are available.

This avoids the circular-wait situation commonly associated with the dining philosophers problem and prevents coders from holding one resource while waiting for another.

### Scheduler blocking and starvation prevention

Requests are stored in a priority heap.

For `fifo`, priority is determined by request sequence.

For `edf`, priority is determined first by the coder's burnout deadline and then by request sequence when deadlines are equal.

A request whose required dongles are temporarily unavailable remains pending instead of being discarded.

The scheduler is non-preemptive. If the highest-priority request is temporarily blocked by unavailable dongles, another compatible request may proceed while the blocked request remains pending.

The scheduling policy is still respected when coders compete for the same dongles.

### Dongle cooldown

Each dongle stores the earliest time at which it may be reused.

When a coder releases its dongles, their next available time is calculated as:

```c
release_time + dongle_cooldown
```

A request cannot be granted until both required dongles are no longer reserved and their cooldown periods have expired.

Timed condition-variable waits are used when a thread is waiting for a cooldown to finish, avoiding unnecessary continuous polling.

### Burnout detection

Each coder stores the timestamp (as a state) at which its latest compile began.

The monitor thread continuously checks:

```text
current_time - last_compile_start >= time_to_burnout
```

Timing is measured with `CLOCK_MONOTONIC`, so elapsed-time calculations are not affected by changes to the system clock.
Coders that have already completed the required number of compiles are excluded from burnout checks.

### Log serialization

Multiple coder threads may attempt to print state changes at approximately the same time.
Logging is protected by the simulation mutex so that only one thread prints a state message at a time.

This prevents output such as two coder messages being mixed together on the same line.

## Thread synchronization mechanisms

Codexion uses POSIX mutexes and condition variables to coordinate coder threads, the scheduler, and the monitor. No separate custom event abstraction is used; event-style synchronization is handled directly with `pthread_cond_t`.

### `pthread_mutex_t`

Two main mutexes are used:

- The **scheduler mutex** protects the request heap, dongle reservation state, cooldown timestamps, and resource granting/releasing.
- The **simulation mutex** protects the stop flag, compile counters, last compile timestamps, and serialized logging.

For example, race conditions are prevented by checking and reserving both required dongles while holding the scheduler mutex, so two coders cannot reserve the same dongle at the same time.

### `pthread_cond_t`

Condition variables allow threads to sleep while waiting for state changes instead of busy-waiting.

The scheduler condition variable wakes coders when resources are released, cooldowns expire, or the simulation stops.

The simulation condition variable is used for simulation-level events such as termination and the single-coder case.

### Thread-safe communication

Coder threads submit requests through the protected scheduler and wait until their required dongles are granted.

For example, when the monitor detects a burnout, it sets the protected stop flag and broadcasts the condition variables so waiting coder threads wake up, observe the stop state, and terminate safely.

Together, mutexes, condition variables, and protected access to shared state provide thread-safe communication between coder threads and the monitor.coder threads and the monitor thread-safe.

## Resources

The following references were used while studying the concepts required for this project:

- [POSIX pthread documentation](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html)
- [Threads and Processes](https://youtu.be/OrM7nZcxXZU?si=r5IWIsd-wTfLwA1u)
- [Unix Threads Playlist](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
- [Heap Data Structure](https://youtu.be/HqPJF2L5h9U?si=QsrClCrws36iRAwu)
- [Race Conditions](https://youtu.be/MqnpIwN7dz0?si=30fXxEQyeEtUYd13)
- [Dining Philosophers Problem](https://youtu.be/FYUi-u7UWgw?si=qG37hZq6FECpyUEB)
- [Process vs Thread — A Practical Comparison Using C Programs](https://process-vs-threads.hashnode.dev/process-vs-thread-a-practical-comparison-using-c-programs)
- [Codexion reference repository](https://github.com/noorhanalayyoub/codexion)

### AI Usage

AI tools were used as a learning and review aid during the project.

AI was used to:

- clarify general concepts related to threads, mutexes, condition variables, race conditions, scheduling, heaps, and shared resources;
- discuss FIFO and EDF scheduling behavior and edge cases;
- suggest regular, edge-case, and stress test inputs;
- review program output to help identify suspicious synchronization or scheduling behavior;
- help reason about debugging observations while the implementation itself was developed and modified manually;
- rephrase, organize, and improve the wording of this README.

AI-generated suggestions were reviewed and tested against the project requirements and actual program behavior before being incorporated.