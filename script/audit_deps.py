#!/usr/bin/env python

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/.

import sys
import json
import argparse
import subprocess

whitelisted_advisories = [
    1523,  # Remove when https://github.com/brave/brave-browser/issues/10584 is resolved
]


def main():
    args = parse_args()
    return audit_deps(args)


def audit_deps(args):
    npm_cmd = 'npm'
    if sys.platform.startswith('win'):
        npm_cmd = 'npm.cmd'

    npm_args = [npm_cmd, 'audit']

    # Just run audit regularly if --audit_dev_deps is passed
    if args.audit_dev_deps:
        return subprocess.call(npm_args)

    npm_args.append('--json')
    audit_process = subprocess.Popen(npm_args, stdout=subprocess.PIPE)
    output, error_data = audit_process.communicate()

    try:
        # results from audit
        result = json.loads(str(output))
    except ValueError:
        # This can happen in the case of an NPM network error
        print('Audit failed to return valid json')
        return 1

    # remove the results which match the exceptions
    for i, val in enumerate(result['actions']):
        result['actions'][i]['resolves'] = \
            [d for d in result['actions'][i]['resolves'] if
                d['id'] not in whitelisted_advisories]

    resolutions, non_dev_exceptions = extract_resolutions(result)

    # Trigger a failure if there are non-dev exceptions
    if non_dev_exceptions:
        print('Result: Audit finished, vulnerabilities found')
        print(json.dumps(non_dev_exceptions, indent=4))
        return 1

    # Still pass if there are dev exceptions, but let the user know about them
    if resolutions:
        print('Result: Audit finished, there are dev package warnings')
        print(json.dumps(resolutions, indent=4))
    else:
        print('Result: Audit finished, no vulnerabilities found')
    return 0


def extract_resolutions(result):
    if 'actions' not in result:
        return [], []

    if len(result['actions']) == 0:
        return [], []

    if 'resolves' not in result['actions'][0]:
        return [], []

    resolutions = result['actions'][0]['resolves']
    return resolutions, [r for r in resolutions if not r['dev']]


def parse_args():
    parser = argparse.ArgumentParser(description='Audit brave-core npm deps')
    parser.add_argument('--audit_dev_deps',
                        action='store_true',
                        help='Audit dev dependencies')
    return parser.parse_args()


if __name__ == '__main__':
    sys.exit(main())
