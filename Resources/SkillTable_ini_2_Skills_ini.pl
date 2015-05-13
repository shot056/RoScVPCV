#!/usr/bin/perl

use strict;
use warnings;
use Data::Dumper;
main();

sub main {
    my $in  = $ARGV[0];
    $in = 'SkillTable.ini' unless(defined $in and length($in) and -f $in);
    my %datas;
    {
        my $flg = 0;
        open(IN , $in) or die "can not open $in\n";
        while(my $line = <IN>) {
            chomp($line);
            if($line =~ /^\[([0-9a-zA-Z]+)\]$/) {
                if($1 eq 'SkillTable') {
                    $flg = 1;
                    print "$line : set load flag : flg = $flg\n";
                }
                else {
                    $flg = 0;
                    print "$line : unset load flag : flg = $flg\n";
                }
            }
            elsif($line =~ /^([^=]+)=([0-9-]+):([^:]+):[0-9-]+:[0-9-]+:[0-9-]+:[0-9-]+$/) {
                next unless($flg);
                print "$line : $1,$2,$3\n";
                $datas{$2} = { str => $1, id => $2, name => $3 };
            }
        }
        close(IN);
    }
    
    open(OUT, ">Skills.ini") or die "can not open Skills.ini\n";
    print OUT "[ID2STR]\n";
    foreach my $id (sort { $a <=> $b } keys(%datas)) {
        print OUT sprintf("%d=%s\n", $id, $datas{$id}->{'str'});
    }
    print OUT "\n";
    print OUT "[ID2NAME]\n";
    foreach my $id (sort { $a <=> $b } keys(%datas)) {
        print OUT sprintf("%d=%s\n", $id, $datas{$id}->{'name'} );
    }
    print OUT "\n";
    close(OUT);
}
