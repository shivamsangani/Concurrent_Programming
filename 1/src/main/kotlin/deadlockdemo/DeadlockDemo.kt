package deadlockdemo

class Pusher(private val list: ThreadSafeList<Int>, private val numOperations: Int) : Runnable {
    override fun run() {
        repeat(numOperations) { i -> list.push(i) }
    }
}

class Popper(private val list: ThreadSafeList<Int>, private val numOperations: Int) : Runnable {
    override fun run() {
        repeat(numOperations) { list.pop() }
    }
}

fun main() {
    val list = ThreadSafeList<Int>()
    val numOperations = 5

    val pusher = Thread(Pusher(list, numOperations))
    val popper = Thread(Popper(list, numOperations))

    pusher.start()
    popper.start()

    pusher.join()
    popper.join()

    println(list.log)
}
