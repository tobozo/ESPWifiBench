# ESPWifiBench

Compare WiFi performances between different ESP modules

- Runs 100 scans of your neighbourhood and for each scan Stores MAC addresses and rssi.
- Outputs some statistics on the scan results and the average RSSi for each Mac.


Output example:

```

Scanned 100 rounds
Total APs found: 4
Average APs found 3 / highest: 4 / lowest: 2

Average RSSI for 5C:13:8E:CE:55:62 is -47 (3 hits)
Average RSSI for 00:19:7A:96:54:7B is -44 (3 hits)
Average RSSI for 52:C3:8E:CE:55:62 is -47 (2 hits)
Average RSSI for 01:37:B7:7B:5F:12 is -51 (4 hits)

```

`Total APs found` how many unique APs found during the 100 rounds
`highest` the highest amount of AP returned in all 100 rounds
`hits` how many times this mac address showed up in the 100 rounds
