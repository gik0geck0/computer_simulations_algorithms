#!/usr/bin/env ruby

def make_lehmer_gen(a, m)
  return lambda { |x| return a*x % m }
end

def lehmer_func(a, m, x)
  return a*x % m
end

class StreamedLehmerGen
  attr_reader :seeds

  def initialize(a, m, j, streams, x0)
    @a = a
    @m = m
    @streams = streams
    @seeds = [0] * streams
    @my_gen = make_lehmer_gen(@a, @m)
    plant_seeds(x0)
  end

  def plant_seeds(first_seed)
    @seeds[0] = first_seed
    q = (@m / @a)
    r = (@m % @a)

    (1...(@streams)).to_a.each do |i|
      x = (@a * (@seeds[i-1] % q)) - r * (@seeds[i-1] / q)

      if x > 0
        @seeds[i] = x
      else
        @seeds[i] = x + @m
      end
    end
  end

  def get_random(stream)
    mgen = @my_gen
    @seeds[stream] = mgen.(@seeds[stream])
    return @seeds[stream]
  end

end

def get_jump_mult(a, j, m)
  return a**j % m
end

m = 2**63 - 1
if ARGV.length < 5
  puts "Required arguments: a j s x0"
end

if ARGV.include?('--aj')
  index = 0
  aj = ARGV[0].to_i
else
  aj = get_jump_mult(ARGV[0].to_i, ARGV[1].to_i, m)
end

stream_gen = StreamedLehmerGen.new(aj, m, ARGV[1].to_i, ARGV[2].to_i, ARGV[3].to_i)

if ARGV.include?('--seeds')
  stream_gen.seeds.each_with_index do |s, i|
    puts "Seed #{i}: #{s}"
  end
else
  (0..(ARGV[2].to_i-1)).to_a.each do |j|
    puts "Stream #{j}"
    (0...ARGV[4].to_i).to_a.each do |i|
      puts stream_gen.get_random(j)
    end
  end
end
