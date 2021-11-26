/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
package cache

import (
	"path"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

func TestCacheLayout(t *testing.T) {
	indexPath := config.ReadTrueBlocks().Settings.IndexPath
	cachePath := config.ReadTrueBlocks().Settings.CachePath
	tests := []struct {
		on        bool
		name      string
		cacheType CacheType
		param     string
		expected  CachePath
		path      string
		wantErr   bool
	}{
		{
			on:    true,
			name:  "index path",
			param: "0010000000-0010200000",
			expected: CachePath{
				Type:      IndexChunk,
				RootPath:  indexPath,
				Subdir:    "finalized/",
				Extension: ".bin",
			},
			path:    "finalized/0010000000-0010200000.bin",
			wantErr: false,
		},
		{
			on:    true,
			name:  "blooms path",
			param: "0010000000-0010200000",
			expected: CachePath{
				Type:      BloomChunk,
				RootPath:  indexPath,
				Subdir:    "blooms/",
				Extension: ".bloom",
			},
			path:    "blooms/0010000000-0010200000.bloom",
			wantErr: false,
		},
		{
			on:    false,
			name:  "blocks cache path",
			param: "001001001",
			expected: CachePath{
				Type:      BlockCache,
				RootPath:  cachePath,
				Subdir:    "blocks/",
				Extension: ".bin",
			},
			path:    "blocks/00/10/01/001001001.bin",
			wantErr: false,
		},
	}

	for _, tt := range tests {
		if !tt.on {
			continue
		}

		t.Run(tt.name, func(t *testing.T) {
			cachePath := &CachePath{}
			cachePath.New(tt.expected.Type)

			if cachePath.Extension != tt.expected.Extension {
				t.Error("Wrong extension", cachePath.Extension)
			}

			if cachePath.Subdir != tt.expected.Subdir {
				t.Error("Wrong subdir", cachePath.Subdir)
			}

			p := cachePath.GetFullPath(tt.param)
			if p != path.Join(tt.expected.RootPath, tt.path) {
				t.Error("Wrong full path", p)
			}
		})
	}
}

// Valid cache paths from the C++ code
// IndexFn:      $INDEX_PATH/finalized/0010000000-0010200000.bin
// BloomFn:      $INDEX_PATH/blooms/0010000000-0010200000.bloom
// BlockFn:      $CACHE_PATH/blocks/00/10/01/001001001.bin
// TxFn:         $CACHE_PATH/txs/00/10/01/001001001-00020.bin
// TraceFn:      $CACHE_PATH/traces/00/10/01/001001001-00020-10.bin
// NeighborFn:   $CACHE_PATH/neighbors/00/10/01/001001001-00020.bin
// ReconFn:      $CACHE_PATH/recons/c011/a724/00e58ecd99ee497cf89e3775d4bd732f/000000012.00013.bin

// IndexPath:    $INDEX_PATH/finalized/
// BloomPath:    $INDEX_PATH/blooms/
// BlockPath:    $CACHE_PATH/blocks/00/10/01/
// TxPath:       $CACHE_PATH/txs/00/10/01/
// TracePath:    $CACHE_PATH/traces/00/10/01/
// NeighborPath: $CACHE_PATH/neighbors/00/10/01/
// ReconPath:    $CACHE_PATH/recons/c011/a724/00e58ecd99ee497cf89e3775d4bd732f/
