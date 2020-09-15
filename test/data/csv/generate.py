#!/usr/bin/env python3

from random import random
import csv
import argparse

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('rows', metavar='r', type=int, help='number of rows to generate') 
    parser.add_argument('columns', metavar='c', type=int, help='number of columns to generate') 
    args = parser.parse_args()
    return args.rows, args.columns

def write_csv(rows: int, columns: int):
    fieldnames = [str("p") + str(i) for i in range(columns)]
    writer = csv.DictWriter(open("test_data.csv", "w"), fieldnames=fieldnames)
    for _ in range(rows):
        d = dict([])
        for i in range(columns):
            d[str("p") + str(i)] = random()
        writer.writerow(d)

def main():
    rows, columns = parse_args()
    write_csv(rows, columns)

if __name__ == '__main__':
    main()


