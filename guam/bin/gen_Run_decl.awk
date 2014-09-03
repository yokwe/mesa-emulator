#!/usr/bin/awk -f

BEGIN {
  no = 1;
}

/@no@/ {
  if (0 < gsub(/@no@/, no)) no++
}

{ print }
