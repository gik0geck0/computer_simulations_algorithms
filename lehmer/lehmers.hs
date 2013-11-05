
-- Original Lehmer random number generator

lehmerRNG :: Int -> Int -> Int -> Int
lehmerRNG a m x = a*x `mod` m

makeInfLehmerGen :: Int -> Int -> Int -> [Int]
makeInfLehmerGen a m seed = iterate (lehmerRNG a m) seed

getJumpSeeds :: Int -> Int -> Int -> Int -> Int -> [Int]
getJumpSeeds a j streams m x = take streams $ makeInfLehmerGen (calcAj a j m) m x

-- Calculates the jump multiplier that should be used: a^j % m
calcAj :: Int -> Int -> Int -> Int
calcAj a j m = fromIntegral $ modExp (toInteger a) (toInteger j) (toInteger m) 1

-- Binary Modular Exponentiation. Pseudo-code taken from http://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method
modExp :: Integer -> Integer -> Integer -> Integer -> Integer
modExp b 0 m r = r
modExp b e m r | e `mod` 2 == 1 = modExp (b*b `mod` m) (e `div` 2) m (r*b `mod` m)
modExp b e m r = modExp (b*b `mod` m) (e `div` 2) m r
-- Note about shifting:          e `div` 2 is the same as e >> 1

main = do
    -- These are the values that the author chose for his 32bit LRNG
    -- a=48271, m=2147483647, streams=256, AJ should be 22925 and x0=123456789
    let a = 48271
        m = 2^31-1
        s = 256
        j = 16775552
        -- infGen = makeInfLehmerGen 
        jSeeds = getJumpSeeds 3 2 2 31 2
        in print $ calcAj a j m
            putStrLn $ show $ take s jSeeds
        --print $ take 10 jSeeds
