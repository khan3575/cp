#!/usr/bin/env python3
"""Random test generator. Usage: python gen.py <seed>"""
import random, sys

rnd = random.Random(int(sys.argv[1]) if len(sys.argv) > 1 else 0)

n = rnd.randint(1, 8)
print(n)
print(*[rnd.randint(1, 10) for _ in range(n)])
