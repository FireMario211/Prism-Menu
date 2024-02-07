export interface UserData {
    userID: number;
    userName: string;
    token: string; // based on a hashed ip addr
    timestamp: string;
};

export interface ThemeData {
    timestamp: string;
    userID: number;
    name: string;
    desc: string;
    screenshots?: Array<string>;
    data: string;
};

export interface MacroData {
    timestamp: string;
    userID: number;
    name: string;
    levelID: string;
    data: string;
};
