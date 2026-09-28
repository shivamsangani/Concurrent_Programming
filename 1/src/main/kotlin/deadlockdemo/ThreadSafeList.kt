package deadlockdemo

import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.withLock

class ThreadSafeList<T> {
    private val list = mutableListOf<T>()
    private val _log = StringBuilder()

    fun push(item: T) {
        // TODO: implement push
        throw NotImplementedError()
    }

    fun pop(): T? {
        // TODO: implement pop
        throw NotImplementedError()
    }

    val log: String get() = _log.toString()
}
