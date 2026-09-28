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

    // TODO: Spawn threads for summing, and join them

    println(total)
}
