import { Router, Request, Response } from 'express'
import crypto from 'crypto'
import multer from 'multer'

const router = Router();

router.post('/api/themes', (req: Request, res: Response) => {
    const { token, data } = req.body;
    
})

export default router;
