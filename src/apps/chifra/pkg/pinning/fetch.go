package pinning

import (
	"context"
	"fmt"
	"io"
	"log"
	"net/http"
	"strconv"
)

// FetchResult type make it easier to return both download content and
// download size information (for validation purposes)
type FetchResult struct {
	Body       io.ReadCloser
	ContentLen int64 // download size in bytes
}

// FetchFromGateway downloads a chunk from an IPFS gateway using HTTP
func FetchFromGateway(ctx context.Context, url string) (*FetchResult, error) {
	request, err := http.NewRequestWithContext(ctx, "GET", url, nil)
	if err != nil {
		log.Fatalln("NewRequestWithContext failed in FetFromGateway with", url)
		return nil, err
	}
	response, err := http.DefaultClient.Do(request)
	if err != nil {
		log.Fatalln("DefaultClient.Do failed in FetFromGateway with", url)
		return nil, err
	}
	if response.StatusCode != 200 {
		log.Fatalln("DefaultClient.Do returned StatusCode not equal to 200 in FetFromGateway with", url)
		return nil, fmt.Errorf("wrong status code: %d", response.StatusCode)
	}
	body := response.Body

	contentLen, err := strconv.ParseInt(response.Header.Get("Content-Length"), 10, 64)
	if err != nil {
		log.Fatalln("Could not parse Content-Length in FetFromGateway with", url)
		return nil, err
	}

	return &FetchResult{
		Body:       body,
		ContentLen: contentLen,
	}, nil
}
