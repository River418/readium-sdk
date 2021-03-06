//
//  owned_by.h
//  ePub3
//
//  Created by Jim Dovey on 2013-05-03.
//  Copyright (c) 2014 Readium Foundation and/or its licensees. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without modification, 
//  are permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this 
//  list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, 
//  this list of conditions and the following disclaimer in the documentation and/or 
//  other materials provided with the distribution.
//  3. Neither the name of the organization nor the names of its contributors may be 
//  used to endorse or promote products derived from this software without specific 
//  prior written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
//  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
//  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
//  OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef __ePub3__owned_by__
#define __ePub3__owned_by__

#include <memory>
#include <type_traits>

#if EPUB_COMPILER_SUPPORTS(CXX_STATIC_ASSERT)
# include <ePub3/utilities/pointer_type.h>
#endif

EPUB3_BEGIN_NAMESPACE

template <class _Tp>
class OwnedBy
{
    // _Tp must be a subclass of PointerType -- hard to assert this though (what template arg? include header, etc?)
//#if EPUB_COMPILER_SUPPORTS(CXX_STATIC_ASSERT)
//    static_assert(std::is_base_of<PointerType<_Tp>, _Tp>::value, "OwnedBy's template parameter must be a subclass of PointerType");
//#endif
    
protected:
    typedef std::shared_ptr<typename std::remove_pointer<_Tp>::type> shared_type;
    typedef std::weak_ptr<typename std::remove_pointer<_Tp>::type>   weak_type;
    
private:
    weak_type       _owner;
    
protected:
    OwnedBy(const shared_type& __owner) : _owner(__owner) {}
    OwnedBy(const OwnedBy& __o) : _owner(__o._owner) {}
    OwnedBy(OwnedBy&& __o) : _owner(std::move(__o._owner)) {}
    virtual ~OwnedBy() {}
    
    OwnedBy& operator=(const OwnedBy& __o) { _owner = __o._owner; return *this; }
    OwnedBy& operator=(OwnedBy&& __o) { _owner = std::move(__o._owner); return *this; }
    
public:
    shared_type Owner() const { return _owner.lock(); }
    bool Orphaned() const { return _owner.expired(); }
    
    void SetOwner(const shared_type __owner) _NOEXCEPT { _owner = __owner; }
#if EPUB_COMPILER_SUPPORTS(CXX_DEFAULT_TEMPLATE_ARGS)
    template <class _Yp,
              class = typename std::enable_if
                      <
                        std::is_convertible<_Yp*, _Tp*>::value
                      >::type
             >
    void SetOwner(const std::shared_ptr<_Yp> __owner) _NOEXCEPT { _owner = __owner; }
#else
public:
    template <class _Yp>
    void SetOwner(const std::shared_ptr<_Yp> __owner,
                  typename std::enable_if
                  <
                    std::is_convertible<_Yp*, _Tp*>::value
                  >::type ** = 0) _NOEXCEPT {
        _owner = __owner;
    }
#endif
    
};

EPUB3_END_NAMESPACE

#endif /* defined(__ePub3__owned_by__) */
