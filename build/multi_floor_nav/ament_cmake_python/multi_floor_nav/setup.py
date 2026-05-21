from setuptools import find_packages
from setuptools import setup

setup(
    name='multi_floor_nav',
    version='0.0.2',
    packages=find_packages(
        include=('multi_floor_nav', 'multi_floor_nav.*')),
)
