export default interface UserData {
    userID: number;
    userName: string;
    token: string; // based on a hashed ip addr
    timestamp: string;
};
