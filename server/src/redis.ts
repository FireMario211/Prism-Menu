// taken from my discord bot 

// https://stackoverflow.com/a/74535407
import type { RedisClientType } from 'redis'
import { createClient }         from 'redis'

let redisClient:                RedisClientType
let isReady:                    boolean
async function getCache(): Promise<RedisClientType> {
    if (!isReady) {
        redisClient = createClient({
            url: 'redis://127.0.0.1:6379',
        })
        redisClient.on('error', err => console.error(`[Redis] Error: ${err}`))
        redisClient.on('connect', () => console.log('[Redis] Connected!'))
        redisClient.on('reconnecting', () => console.log('[Redis] Reconnecting...'))
        redisClient.on('ready', () => {
            isReady = true
            console.log('[Redis] Ready!')
        })
        await redisClient.connect();
    }
    return redisClient
}
getCache().then(connection => {
    redisClient = connection
}).catch(err => {
    console.error('Failed to connect to Redis', err)
})
export { getCache }
