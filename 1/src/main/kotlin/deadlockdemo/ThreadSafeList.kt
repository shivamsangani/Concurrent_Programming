package deadlockdemo

import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.withLock

class ThreadSafeList<T> {
    // The implementation of push and pop in our ThreadSafeList have the possibility of deadlocking.
    // It is possible that one thread acquires the `logLock` in `push`, another thread acquires the `listLock` in `pop`,
    // and both threads are waiting for each other. This condition is called circular wait and leads to a deadlock.
    private val list = mutableListOf<T>()
    private val _log = StringBuilder()

    private val listLock = ReentrantLock()
    private val logLock = ReentrantLock()

    fun push(item: T) {
        logLock.withLock {
            _log.append("Push succeeded")
            listLock.withLock { list.add(item) }
        }
    }

    fun pop(): T? {
        logLock.withLock {
            listLock.withLock {
                if (list.isEmpty()) {
                    _log.append("Pop failed")
                } else {
                    _log.append("Pop succeeded")
                }
                return list.removeFirstOrNull()
            }
        }
    }

    val log: String get() = _log.toString()
}
