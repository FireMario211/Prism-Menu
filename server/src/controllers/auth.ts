import redis from 'redis'
import crypto from 'crypto';
import User from '../components/User';

const client = redis.createClient(); // switch to cache

export function hashIP(ip: string): string {
    const hash = crypto.createHash('sha256');
    hash.update(ip);
    return hash.digest('hex');
}

export async function getUser(hashIP: string): Promise<User | null> {
    try {
        const userData = (await client.hGetAll(`users:${hashIP}`) as unknown) as User;
        if (userData) return userData;
        return null;
    } catch (e) {
        console.error(e);
        return null;
    }
}

export async function createUser(name: string, hashIP: string): Promise<User | null> {
    return new Promise((resolve, reject) => {
        return getUser(hashIP).then(user => {
            if (user != null) return reject("Already created an account.");
            resolve();
        }).catch(reject);
    })
}
