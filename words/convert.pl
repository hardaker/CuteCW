#!/usr/bin/perl

my ($num) = ($ARGV[0] =~ /^(...)/);

print "words[N$num] = new QList<QString>;\n";
while(<>) {
    chomp;
    print "words[N$num]->push_back(\"$_\");\n";
}
