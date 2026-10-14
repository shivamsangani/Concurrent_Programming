package dataracedemo

import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.thread
import kotlin.concurrent.withLock

fun main() {
    val numElements: Int = 1 shl 24 //2^24 elements
    val numThreads: Int = 1 shl 12  //4 threads
    val elemsPerThread: Int = numElements / numThreads

    val data: List<Int> = List(numElements) { 1 }

    var total = 0L
    val lock = ReentrantLock()

    // TODO: Spawn threads for summing, and join them
    val threads = (0 until numThreads).map { i -> 
        thread {
            val partialSum = data.subList(i * elemsPerThread, (i+1) * elemsPerThread).sum()
            lock.withLock { total += partialSum}
        }
    }


    threads.forEach { it.join() }

    println(total)
}
