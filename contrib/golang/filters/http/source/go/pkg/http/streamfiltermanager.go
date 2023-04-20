/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package http

import (
	"github.com/envoyproxy/envoy/contrib/golang/filters/http/source/go/pkg/api"
)

type streamFilterConfig struct {
	name   string
	config interface{}
}

type streamFilterManagerConfig struct {
	filters []streamFilterConfig
}

type streamFilterManager struct {
	callbacks  api.FilterCallbackHandler
	filters    []api.StreamFilter
	headerIdx  int
	dataIdx    int
	TrailerIdx int
}

func (f *streamFilterManager) DecodeHeaders(header api.RequestHeaderMap, endStream bool) api.StatusType {
	for f.headerIdx < len(f.filters) {
		f.headerIdx++
		filter := f.filters[f.headerIdx]
		status := filter.DecodeHeaders(header, endStream)
		if status != api.Continue {
			return status
		}
	}
	return api.Continue
}

func (f *streamFilterManager) DecodeData(buffer api.BufferInstance, endStream bool) api.StatusType {
	return api.Continue
}

func (f *streamFilterManager) DecodeTrailers(trailers api.RequestTrailerMap) api.StatusType {
	return api.Continue
}

func (f *streamFilterManager) EncodeHeaders(header api.ResponseHeaderMap, endStream bool) api.StatusType {
	return api.Continue
}

func (f *streamFilterManager) EncodeData(buffer api.BufferInstance, endStream bool) api.StatusType {
	return api.Continue
}

func (f *streamFilterManager) EncodeTrailers(trailers api.ResponseTrailerMap) api.StatusType {
	return api.Continue
}

func (f *streamFilterManager) OnDestroy(reason api.DestroyReason) {
}

func streamFilterManagerFactory(config interface{}) api.StreamFilterFactory {
	conf := config.(streamFilterManagerConfig)
	return func(callbacks api.FilterCallbackHandler) api.StreamFilter {
		fm := streamFilterManager{
			callbacks: callbacks,
		}

		for filterConfig := range conf.filters {
			filter := filterFactory(filterConfig)
			fm.filters = append(fm.filters, filter)
		}

		return &fm
	}
}
