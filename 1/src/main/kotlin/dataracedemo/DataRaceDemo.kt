package dataracedemo

import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.thread
import kotlin.concurrent.withLock

fun main() {
    val numElements: Int = 1 shl 24
    val numThreads: Int = 1 shl 2
    val elemsPerThread: Int = numElements / numThreads

    val data: List<Int> = List(numElements) { 1 }

    var total = 0L
    val lock = ReentrantLock()

    // TODO: Spawn threads for summing, and join them
    val threads = (0 until numThreads).map { i ->
        thread {
            val partialSum = data.subList(i * elemsPerThread, (i + 1) * elemsPerThread).sum()
            // Here there is a Write-Read data race on total.
            total += partialSum
            // This can be resolved by using a lock:
            // lock.withLock { total += partialSum }
        }
    }

    threads.forEach { it.join() }

    println(total)
}
