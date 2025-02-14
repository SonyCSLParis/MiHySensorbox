import argparse
from experiment import ex as experiment

parser = argparse.ArgumentParser()
parser.add_argument('id')
args = parser.parse_args()
print(f'Listening for data of system {args.id}')

experiment.add_config({'system-id': args.id})
experiment.run()

