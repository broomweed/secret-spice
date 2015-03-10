# This script converts an image into a tilemap using ImageMagick!
#!/usr/bin/env perl

use Capture::Tiny ":all";
use File::Copy "move";

my $filename = $ARGV[0];
my $folder = $ARGV[1];

my $dims = `convert $filename -format "%wx%h" info:`;
$dims =~ /(\d+)x(\d+)/;
my $width = $1/16;
my $height = $2/16;

print "Image width in tiles: $width; height: $height\n";

`mkdir $folder`;
`convert $filename -crop 16x16 +repage +adjoin $folder/tile_%04d.png`;
my @files = <$folder/tile_*.png>;
my @stuff;

# check for non-tile-shaped tiles
my $lastfile;
for my $file (@files) {
    print $file;
    my $info = `convert $file -format "%wx%h" info:`;
    if ($info ne "16x16\n") {
        push @stuff, $file;
    }
    print "\rChecking tile size: ";
    $lastfile = $file;
}
print "done. (last: $lastfile)\n";

my @ids = (0..@files-1);

# remove duplicates
my $dups = 0;
for my $file (@files) {
    next if $file ~~ @stuff;
    if (not -e $file) {
        print "\rChecking for duplicates: $file SKIP            ";
        next;
    }
    $file =~ /_(\d+)\.png/;
    $fileID = 0+$1;
    for my $comp (@files) {
        print "\rChecking for duplicates: ";
        print "$file vs $comp";
        next if $comp ~~ @stuff;
        next if $file eq $comp;
        next if not -e $comp;
        $comp =~ /_(\d+)\.png/;
        $compID = 0+$1;
        my ($thing, $info, @res) = capture {
            system("compare -metric RMSE $file $comp NULL");# 1>/dev/null 2>&1");
        };
        $info =~ /^(\d+)/;
        my $compnum = $1;
        if ($compnum == 0) {
            $ids[$compID] = $fileID;
            $dups++;
            unlink $comp;
            if ($!) {
                print "\r$comp is a duplicate of $file, but failed to delete it\n";
            } else {
                print "\r$comp is a duplicate of $file                         \n";
            }
        }
    }
}
print "\rChecking for duplicates: done. ($dups duplicates found)                        \n";
print "Recounting tile numbers...";

@files = <$folder/tile_*.png>;
my $lastnum = -1;
my $offset = 0;
for my $file (@files) {
    $file =~ /tile_(\d+).png/;
    my $num = $1;
    print "#", $1;
    if ($num - $lastnum > 1) {
        for (@ids) {
            @ids = map {
                if ($_ == $num) {
                    $lastnum+1;
                } else {
                    $_;
                }
            } @ids;
        }
        $num = $lastnum + 1;
        my $newname = sprintf "$folder/tile_%04d.png", $num;
        #print "$file --> $newname\n";
        rename $file, $newname;
    }
    print "\rRecounting tile numbers...";
    $lastnum = $num;
}
print "done.\n";

@files = <$folder/tile_*.png>;
print 'Stitching images together...';
`convert @files +append $folder/tiles.png`;

print 'Result: ', join (',', @ids), "\n";
open my $savefile, '>', "$folder/tilemap.txt";
print { $savefile } "Width in tiles: $width\nheight in tiles: $height\n", join ',', @ids;
