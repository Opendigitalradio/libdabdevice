#!/usr/bin/env python3
# pylint: disable=invalid-name,missing-docstring,redefined-builtin
# -*- coding: utf-8 -*-

import os
import subprocess

subprocess.call('cd ../../ && doxygen', shell=True)

html_theme = 'sphinx_rtd_theme'

extensions = [
    'breathe',
    'sphinx.ext.todo',
]

breathe_projects = {
    'DABDevice': os.path.abspath('../xml')
}
breathe_default_project = 'DABDevice'

source_suffix = '.rst'
master_doc = 'index'

project = 'DABDevice'
copyright = '2016, Felix Morgner and Tobias Stauber'
author = 'Felix Morgner, Tobias Stauber'

version = '1.0'
release = '1.0.3'

show_authors = False

language = 'en'

exclude_patterns = [
    'build',
    'Thumbs.db',
    '.DS_Store',
]

pygments_style = 'sphinx'

todo_include_todos = True

htmlhelp_basename = 'DABDevicedoc'

man_pages = [
    (master_doc, 'DABDevice', 'DABDevice Documentation',
     [author], 1)
]

primary_domain = 'cpp'
